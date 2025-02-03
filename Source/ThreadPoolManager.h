#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

class FThreadPoolManager
{
public:
	FThreadPoolManager(int InThreadsNum);
	~FThreadPoolManager();
	void AddTask(void(*InTask)(void*), void* InData);

protected:
	std::vector<std::thread> RunningThreads;
	std::queue<std::pair<void(*)(void*), void*>> Tasks;
	std::mutex Mutex;
	std::condition_variable ConditionVariable;
	bool bShouldWeStopRunningThreads = false;
};