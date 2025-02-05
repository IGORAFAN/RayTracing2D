#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

class FThreadPoolManager
{
public:
	FThreadPoolManager(int InThreadsNum);
	~FThreadPoolManager();
	void AddTask(std::function<void(void*)> Task, void* InData);
	void WaitUntilAllTasksFinished();

protected:
	std::vector<std::thread> RunningThreads;
	std::queue<std::pair<std::function<void(void*)>, void*>> Tasks;
	std::atomic<unsigned int> TaskCount;
	std::mutex Mutex;
	std::condition_variable ConditionVariable;
	bool bShouldWeStopRunningThreads = false;
};