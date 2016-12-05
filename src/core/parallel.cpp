/*
 * parallel.cpp
 *
 *  Created on: 2016年8月10日
 *      Author: zhuqian
 */

#include "parallel.h"

void EnqueueTasks(const std::vector<Task *> &tasks) {

	if(CORE_NUM==1){
		numUnfinishedTasks += tasks.size();
		progress_thread = new std::thread(progress);
		for(int i=0;i<tasks.size();++i){
			tasks[i]->Run();
			--numUnfinishedTasks;
		}
		return;
	}

	taskQueueMutex.lock();
	//cout << "EnqueueTasks" << endl;
	for (int i = 0; i < tasks.size(); ++i) {
		taskQueue.push_back(tasks[i]);
	}
	numMaxTasks=taskQueue.size();
	//cout << "EnqueueTasks end" << endl;
	taskQueueMutex.unlock();
	tasksRunningConditionMutex.lock();
	numUnfinishedTasks += tasks.size();
	tasksRunningConditionMutex.unlock();
	if (!threads){
		InitTasks();
		if(!progress_thread)progress_thread = new std::thread(progress);
	}

//	//	progress_thread->detach();
//	for (int i = 0; i < CORE_NUM; ++i) {
//		threads[i]->detach();
//	}
}


void WaitForAllTasks() {
	std::unique_lock<std::mutex> mux(tasksRunningConditionMutex);
	while (numUnfinishedTasks > 0) {
		std::cout << "waiting tasksRunningCondition" << std::endl;
		tasksRunningCondition.wait(mux);
	}
	//	for (int i = 0; i < CORE_NUM; ++i) {
	//			delete threads[i];
	//	}
	delete[] threads;
	threads=nullptr;
	std::cout << "all tasks OK" << std::endl;
}

const std::thread::id RWMutex::NULL_THEAD;
