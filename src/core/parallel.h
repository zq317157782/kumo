/*
 * parallel.h
 *
 *  Created on: 2016年8月9日
 *      Author: zhuqian
 */

#ifndef CORE_PARALLEL_H_
#define CORE_PARALLEL_H_
#include "global.h"
//并行相关
#define CORE_NUM 4

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
static mutex taskQueueMutex; //任务队列互斥所
static condition_variable tasksRunningCondition;
static mutex tasksRunningConditionMutex;
static thread** threads; //工作线程
static thread* progress_thread; //进度线程
static std::vector<Task *> taskQueue; //任务队列
//执行任务入口
static void taskEntry() {
	while (true) {
		//cout<<"get task"<<endl;
		Task *myTask = nullptr;
		{
			unique_lock<mutex> lock(taskQueueMutex);
			//cout<<"----"<<endl;
			if (taskQueue.size() == 0) {
				active_core -= 1;
				cout << "kill thread " << this_thread::get_id() << endl;
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
			cout << "notify tasksRunningCondition" << endl;
		}
	}
}

static void progress() {
	int maxNum = numUnfinishedTasks;
	while (true) {
		if (numUnfinishedTasks == 0) {
			cout << "kill progress thread" << endl;
			break;
		}
		cout << "[" << ((float) (maxNum - numUnfinishedTasks) / maxNum) * 100
				<< "%] active cores:" << active_core << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}

}

static void InitTasks() {
	//cout<<"init tasks"<<endl;
	threads = new thread*[CORE_NUM];
	for (int i = 0; i < CORE_NUM; ++i) {
		thread*work_thread = new thread(taskEntry);
		threads[i] = work_thread;
	}
}

//插入任务队伍
void EnqueueTasks(const vector<Task *> &tasks);

void WaitForAllTasks();

//读写锁
class RWMutex {
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

class RWMutexLock {
private:
	RWMutex* mMutex;
public:
	RWMutexLock(RWMutex* m) {
		mMutex = m;
		mMutex->readLock();
	}

	~RWMutexLock(){
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

	void upgrade2Writer(){
		mMutex->upgrade2Writer();
	}

	void down2Reader(){
		mMutex->down2Reader();
	}

};

#endif /* CORE_PARALLEL_H_ */
