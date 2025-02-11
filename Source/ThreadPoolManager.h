#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
//#include "../include/concurrent_queue.h"
#include <tbb/concurrent_queue.h>

class FThreadPoolManager
{
public:
	FThreadPoolManager(int InThreadsNum);
	~FThreadPoolManager();
	void AddTask(std::function<void(void*)> Task, void* InData);
	void WaitUntilAllTasksFinished();

protected:
	std::vector<std::thread> RunningThreads;
	//std::queue<std::pair<std::function<void(void*)>, void*>> Tasks;
	tbb::detail::d2::concurrent_queue<std::pair<std::function<void(void*)>, void*>> Tasks;
	std::atomic<unsigned int> TaskCount;
	std::mutex Mutex;
	std::condition_variable ConditionVariable;
	std::atomic<bool> bShouldWeStopRunningThreads;
};