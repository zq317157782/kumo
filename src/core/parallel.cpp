/*
 * parallel.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: zhuqian
 */

#include "parallel.h"

void EnqueueTasks(const vector<Task *> &tasks) {

	if(CORE_NUM==1){
		numUnfinishedTasks += tasks.size();
		progress_thread = new thread(progress);
		for(int i=0;i<tasks.size();++i){
			//cout<<"taskID:"<<numUnfinishedTasks<<endl;
			if(1252==numUnfinishedTasks)
				cout<<"taskID:"<<numUnfinishedTasks<<endl;
			tasks[i]->Run();

			--numUnfinishedTasks;
		}
		return;
	}

	taskQueueMutex.lock();
	cout << "EnqueueTasks" << endl;
	for (int i = 0; i < tasks.size(); ++i) {
		taskQueue.push_back(tasks[i]);
	}
	cout << "EnqueueTasks end" << endl;
	taskQueueMutex.unlock();
	tasksRunningConditionMutex.lock();
	numUnfinishedTasks += tasks.size();
	tasksRunningConditionMutex.unlock();
	if (!threads){
		InitTasks();
		progress_thread = new thread(progress);
	}

//	//	progress_thread->detach();
//	for (int i = 0; i < CORE_NUM; ++i) {
//		threads[i]->detach();
//	}
}


void WaitForAllTasks() {
	unique_lock<mutex> mux(tasksRunningConditionMutex);
	while (numUnfinishedTasks > 0) {
		cout << "waiting tasksRunningCondition" << endl;
		tasksRunningCondition.wait(mux);
	}
}


RWLock::RWLock(bool writeFirst) :
	WRITE_FIRST(writeFirst),
	m_write_thread_id(),
	m_lockCount(0),
	m_writeWaitCount(0) {
}
int RWLock::readLock() {
	// ==时为独占写状态,不需要加锁
	if (this_thread::get_id() != this->m_write_thread_id) {
		int count;
		if (WRITE_FIRST)//写优先模式下,要检测等待写的线程数为0(m_writeWaitCount==0)
			do {
				while ((count = m_lockCount) == WRITE_LOCK_STATUS || m_writeWaitCount > 0);//写锁定时等待
			} while (!m_lockCount.compare_exchange_weak(count, count + 1));
		else
			do {
				while ((count = m_lockCount) == WRITE_LOCK_STATUS); //写锁定时等待
			} while (!m_lockCount.compare_exchange_weak(count, count + 1));
	}
	return m_lockCount;
}
int RWLock::readUnlock() {
	// ==时为独占写状态,不需要加锁
	if (this_thread::get_id() != this->m_write_thread_id)
		--m_lockCount;
	return m_lockCount;
}
int RWLock::writeLock() {
	// ==时为独占写状态,避免重复加锁
	if (this_thread::get_id() != this->m_write_thread_id) {
		++m_writeWaitCount;//写等待计数器加1
						   // 没有线程读取时(加锁计数器为0)，置为-1加写入锁，否则等待
		for (int zero = FREE_STATUS; !this->m_lockCount.compare_exchange_weak(zero, WRITE_LOCK_STATUS); zero = FREE_STATUS);
		--m_writeWaitCount;//获取锁后,计数器减1
		m_write_thread_id = this_thread::get_id();
	}
	return m_lockCount;
}
int RWLock::writeUnlock() {
	if (this_thread::get_id() != this->m_write_thread_id) {
		throw runtime_error("writeLock/Unlock mismatch");
	}
	assert(WRITE_LOCK_STATUS == m_lockCount);
	m_write_thread_id = NULL_THEAD;
	m_lockCount.store(FREE_STATUS);
	return m_lockCount;
}
const std::thread::id RWLock::NULL_THEAD;