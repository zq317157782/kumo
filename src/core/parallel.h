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
#define CORE_NUM 8

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

class RWLock {
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
	/* 初始为0的线程id */
	static const  std::thread::id NULL_THEAD;
	const bool WRITE_FIRST;
	/* 用于判断当前是否是写线程 */
	thread::id m_write_thread_id;
	/* 资源锁计数器,类型为int的原子成员变量,-1为写状态，0为自由状态,>0为共享读取状态 */
	atomic_int m_lockCount;
	/* 等待写线程计数器,类型为unsigned int的原子成员变量*/
	atomic_uint m_writeWaitCount;
public:
	// 禁止复制构造函数
	RWLock(const RWLock&) = delete;
	// 禁止对象赋值操作符
	RWLock& operator=(const RWLock&) = delete;
	//RWLock& operator=(const RWLock&) volatile = delete;
	RWLock(bool writeFirst = false);;//默认为读优先模式
	virtual ~RWLock() = default;
	int readLock();
	int readUnlock();
	int writeLock();
	int writeUnlock();
};

#endif /* CORE_PARALLEL_H_ */
