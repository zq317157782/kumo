/*
 * parallel.h
 *
 *  Created on: 2016年8月9日
 *      Author: zhuqian
 */

#ifndef CORE_PARALLEL_H_
#define CORE_PARALLEL_H_
#include <kumo.h>
//并行相关
#define CORE_NUM 1

static int active_core = CORE_NUM;

//最小任务单位
class Task {
public:
	virtual ~Task() {
	}
	;
	virtual void Run() = 0;
};

static unsigned int numUnfinishedTasks; //未完成的任务数
static unsigned int numMaxTasks;
static std::mutex taskQueueMutex; //任务队列互斥所
static std::condition_variable tasksRunningCondition;
static std::mutex tasksRunningConditionMutex;
static std::thread** threads; //工作线程
static std::thread* progress_thread; //进度线程
static std::vector<Task *> taskQueue; //任务队列
//执行任务入口
static void taskEntry() {
	while (true) {

		//cout<<"get task"<<endl;
		Task *myTask = nullptr;
		{
			std::unique_lock<std::mutex> lock(taskQueueMutex);
			//cout<<"----"<<endl;
			if (taskQueue.size() == 0) {
				active_core -= 1;
				std::cout << "kill thread " << std::this_thread::get_id() << std::endl;
				break;
			}
			myTask = taskQueue.back();
			taskQueue.pop_back();
			lock.unlock();
		}

		myTask->Run(); //正真执行任务的地方
		tasksRunningConditionMutex.lock();
		int unfinished = --numUnfinishedTasks;
		tasksRunningConditionMutex.unlock();
		if (unfinished == 0) {
			tasksRunningCondition.notify_all();
			std::cout << "notify tasksRunningCondition" << std::endl;
		}
	}
}

static void progress() {
	//int maxNum = numUnfinishedTasks;
	while (true) {
		if (numUnfinishedTasks == 0) {
			std::cout << "kill progress thread" << std::endl;
			break;
		}
		std::cout << "[" << ((Float) (numMaxTasks - numUnfinishedTasks) / numMaxTasks) * 100
				<< "%] active cores:" << active_core <<"tasks:"<<numUnfinishedTasks<< std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}

static void InitTasks() {
	//cout<<"init tasks"<<endl;
	threads = new std::thread*[CORE_NUM];
	active_core=CORE_NUM;
	for (int i = 0; i < CORE_NUM; ++i) {
		std::thread*work_thread = new std::thread(taskEntry);
		threads[i] = work_thread;
	}
}

//插入任务队伍
void EnqueueTasks(const std::vector<Task *> &tasks);

void WaitForAllTasks();

//读写锁
class RWMutex {
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
	static const std::thread::id NULL_THEAD;
	const bool WRITE_FIRST;
	std::thread::id m_write_thread_id;
	/* 资源锁计数器,类型为int的原子成员变量,-1为写状态，0为自由状态,>0为共享读取状态 */
	std::atomic_int m_lockCount;
	/* 等待写线程计数器,类型为unsigned int的原子成员变量*/
	std::atomic_uint m_writeWaitCount;
public:
	RWMutex(const RWMutex&) = delete;
	// 禁止对象赋值操作符
	RWMutex& operator=(const RWMutex&) = delete;
	//RWLock& operator=(const RWLock&) volatile = delete;
	RWMutex(bool writeFirst = false) :
			WRITE_FIRST(writeFirst), m_write_thread_id(), m_lockCount(0), m_writeWaitCount(
					0) {
	} //默认为读优先模式
	virtual ~RWMutex() = default;

	int readLock() {
		if (std::this_thread::get_id() != this->m_write_thread_id) {
			int count;
			if (WRITE_FIRST) //写优先模式下,要检测等待写的线程数为0(m_writeWaitCount==0)
				do {
					while ((count = m_lockCount) == WRITE_LOCK_STATUS
							|| m_writeWaitCount > 0)
						; //写锁定时等待
				} while (!m_lockCount.compare_exchange_weak(count, count + 1));
			else
				do {
					while ((count = m_lockCount) == WRITE_LOCK_STATUS)
						; //写锁定时等待
				} while (!m_lockCount.compare_exchange_weak(count, count + 1));
		}
		//cout<<">"<<endl;
		return m_lockCount;
	}

	int readUnlock() {
		// ==时为独占写状态,不需要加锁
		if (std::this_thread::get_id() != this->m_write_thread_id)
			--m_lockCount;
		//cout<<"<"<<endl;
		return m_lockCount;
	}

	int writeLock() {
		// ==时为独占写状态,避免重复加锁
		if (std::this_thread::get_id() != this->m_write_thread_id) {
			++m_writeWaitCount; //写等待计数器加1
			// 没有线程读取时(加锁计数器为0)，置为-1加写入锁，否则等待
			for (int zero = FREE_STATUS;
					!this->m_lockCount.compare_exchange_weak(zero,
					WRITE_LOCK_STATUS); zero = FREE_STATUS)
				;
			--m_writeWaitCount;		        //获取锁后,计数器减1
			m_write_thread_id = std::this_thread::get_id();
		}
		//cout<<"!"<<endl;
		return m_lockCount;
	}

	int writeUnlock() {

		if (std::this_thread::get_id() != this->m_write_thread_id) {
			std::cout << std::this_thread::get_id() << std::endl;
			throw std::runtime_error("writeLock/Unlock mismatch");
		}
		assert(WRITE_LOCK_STATUS==m_lockCount);
		m_write_thread_id = NULL_THEAD;
		m_lockCount.store(FREE_STATUS);
		//cout<<"?"<<endl;
		return m_lockCount;
	}

	void upgrade2Writer() {
		if (std::this_thread::get_id() != this->m_write_thread_id) {
			--m_lockCount;
			++m_writeWaitCount; //写等待计数器加1
			for (int zero = FREE_STATUS;
					!this->m_lockCount.compare_exchange_weak(zero,
					WRITE_LOCK_STATUS); zero = FREE_STATUS)
				;
			--m_writeWaitCount;		        //获取锁后,计数器减1
			m_write_thread_id = std::this_thread::get_id();
			//cout<<"升级读者"<<endl;
		}

//		readUnlock();
//		writeLock();
	}

	void down2Reader() {

		if (std::this_thread::get_id() != this->m_write_thread_id) {
			std::cout << std::this_thread::get_id() << std::endl;
			throw std::runtime_error("writeLock/Unlock mismatch");
		}
		assert(WRITE_LOCK_STATUS==m_lockCount);
		m_write_thread_id = NULL_THEAD;
		m_lockCount.store(FREE_STATUS + 1);
		//cout<<"降级读者"<<endl;
	}
};
/*
 * class RWMutex {
 private:
 mutex innerMtx; //互斥锁
 unsigned int mNumReader;
 bool mIsWriteState;
 condition_variable cond;
 public:
 RWMutex() {
 mNumReader = 0;
 mIsWriteState = false;
 }

 void readLock() {
 unique_lock<mutex> writeLock(innerMtx);
 //	cout<<"读者获得互斥锁"<<endl;
 while (mIsWriteState) {
 //	cout<<"等待写着"<<endl;
 cond.wait(writeLock);
 }
 ++mNumReader; //增加读者
 //cout << "读者LOCK"<< endl;
 }

 void readUnlock() {
 unique_lock<mutex> writeLock(innerMtx);
 if (mIsWriteState) {
 cerr << "write_lock" << endl;
 return;
 }
 --mNumReader;
 if (mNumReader == 0) {
 cond.notify_all();
 }
 //cout << "读者UNLOCK" << endl;
 }

 void writeLock() {
 unique_lock<mutex> readLock(innerMtx);
 //cout<<"写着获得互斥锁"<<endl;
 while (mNumReader > 0 || mIsWriteState) {
 //	cout<<"等待读者"<<endl;
 cond.wait(readLock);
 }
 mIsWriteState = true;
 //cout << "写者LOCK" << endl;
 }

 void writeUnlock() {
 unique_lock<mutex> readLock(innerMtx);
 if (mNumReader > 0) {
 cerr << "read_lock" << endl;
 return;
 }

 if (!mIsWriteState) {
 cerr << "is write_state" << endl;
 return;
 }
 mIsWriteState = false;
 cond.notify_all();
 //cout << "写者UNLOCK" << endl;
 }

 void upgrade2Writer() {
 unique_lock<mutex> writeLock(innerMtx);
 --mNumReader;

 while (mIsWriteState || mNumReader > 0) {
 //cout << "等待成为写着" << endl;
 cond.wait(writeLock);
 }
 mIsWriteState = true;
 //cout << "读者==>写着" << endl;
 }

 void down2Reader() {
 unique_lock<mutex> writeLock(innerMtx);
 if (!mIsWriteState) {
 cerr << "already read_state" << endl;
 return;
 }
 mIsWriteState = false;
 ++mNumReader ;
 //cond.notify_all();
 //cout << "读者<==写着" << endl;
 }
 };
 * */

class RWMutexLock {
private:
	RWMutex* mMutex;
public:
	RWMutexLock(RWMutex* m) {
		mMutex = m;
		mMutex->readLock();
	}

	~RWMutexLock() {
		mMutex->readUnlock();
	}
//	void readLock() {
//		cout<<"lock"<<endl;
//		mMutex->readLock();
//	}
//	void readUnlock() {
//		cout<<"unlock"<<endl;
//		mMutex->readUnlock();
//	}
//	void writeLock() {
//		mMutex->writeLock();
//	}
//	void writeUnlock() {
//		mMutex->writeUnlock();
//	}

	void upgrade2Writer() {
		mMutex->upgrade2Writer();
	}

	void down2Reader() {
		mMutex->down2Reader();
	}

};

#endif /* CORE_PARALLEL_H_ */
