#include "ThreadPoolManager.h"

FThreadPoolManager::FThreadPoolManager(int InThreadsNum) : bShouldWeStopRunningThreads(false)
{
	if (InThreadsNum < 1)
	{
		InThreadsNum = 1;
	}
	for (int i = 0; i < InThreadsNum; ++i)
	{
		RunningThreads.push_back(std::thread(
			[this]()
			{
				while (true)
				{
					std::pair<void(*)(void*), void*> Task;
					{
						std::unique_lock<std::mutex> Lock(Mutex);
						this->ConditionVariable.wait(Lock,
							[this]() -> bool
							{
								if (this)
								{
									return this->bShouldWeStopRunningThreads || !this->Tasks.empty();
								}
								else
								{
									return true;
								}
							}
						);
						if (this->bShouldWeStopRunningThreads && this->Tasks.empty())
						{
							return;
						}
						Task = this->Tasks.front();
						this->Tasks.pop();
					}
					Task.first(Task.second);
				}
			})
		);
	}
}

FThreadPoolManager::~FThreadPoolManager()
{
	{
		std::unique_lock<std::mutex> Lock(Mutex);
		bShouldWeStopRunningThreads = true;
	}

	ConditionVariable.notify_all();

	for (std::thread& Thread : RunningThreads)
	{
		Thread.join();
	}
}

void FThreadPoolManager::AddTask(void(*InTask)(void*), void* InData)
{
	{
		std::unique_lock<std::mutex> Lock(Mutex);
		
		if (bShouldWeStopRunningThreads)
		{
			return;
		}
		
		Tasks.push(std::pair<void(*)(void*), void*>(InTask, InData));

		ConditionVariable.notify_one();
	}
}
