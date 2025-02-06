#include "ThreadPoolManager.h"
#include "Helper.h"

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
					std::pair<std::function<void(void*)>, void*> Task;
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
					this->TaskCount.fetch_sub(1);
					this->ConditionVariable.notify_all();
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

void FThreadPoolManager::AddTask(std::function<void(void*)> InTask, void* InData)
{
	if (!Mutex.try_lock())
	{
		DebugTrace("Attempt to lock was unsuccessful!");
		Mutex.lock();
	}
	//std::unique_lock<std::mutex> Lock(Mutex);

	if (bShouldWeStopRunningThreads)
	{
		return;
	}

	Tasks.push(std::pair<std::function<void(void*)>, void*>(InTask, InData));
	TaskCount.fetch_add(1);

	ConditionVariable.notify_one();

	Mutex.unlock();
}

void FThreadPoolManager::WaitUntilAllTasksFinished()
{
	std::unique_lock<std::mutex> Lock(Mutex);
	ConditionVariable.wait(Lock,
		[this]() -> bool
		{
			return this->TaskCount == 0;
		}
	);
}
