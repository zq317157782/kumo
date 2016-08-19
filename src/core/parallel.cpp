/*
 * parallel.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: zhuqian
 */

#include "parallel.h"

void EnqueueTasks(const vector<Task *> &tasks) {

	if(CORE_NUM==1){
		for(int i=0;i<tasks.size();++i){
			tasks[i]->Run();
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
	if (!threads)
			InitTasks();
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
