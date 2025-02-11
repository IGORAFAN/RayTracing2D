#include "ThreadPoolManager.h"
#include "Helper.h"
#include "ProfileMertickCollector.h"

FThreadPoolManager::FThreadPoolManager(int InThreadsNum)
{
	bShouldWeStopRunningThreads.store(false);

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
								return this->bShouldWeStopRunningThreads.load() || !this->Tasks.empty();
							}
						);
					}

					if (this->bShouldWeStopRunningThreads.load() && this->Tasks.empty())
					{
						return;
					}

					if (this->Tasks.try_pop(Task))
					{
						Task.first(Task.second);
						this->TaskCount.fetch_sub(1);
						this->ConditionVariable.notify_all();
					}
					else
					{
						DebugTrace("Task can't be removed from the tasks queue!");
					}
				}
			})
		);
	}
}

FThreadPoolManager::~FThreadPoolManager()
{
	bShouldWeStopRunningThreads.store(true);

	ConditionVariable.notify_all();

	for (std::thread& Thread : RunningThreads)
	{
		Thread.join();
	}
}

void FThreadPoolManager::AddTask(std::function<void(void*)> InTask, void* InData)
{
	//PROFILE_METRICS_COLLECTOR("FThreadPoolManager_AddTask");

	if (bShouldWeStopRunningThreads.load())
	{
		return;
	}

	Tasks.push(std::pair<std::function<void(void*)>, void*>(InTask, InData));
	TaskCount.fetch_add(1);

	ConditionVariable.notify_one();
}

void FThreadPoolManager::WaitUntilAllTasksFinished()
{
	std::unique_lock<std::mutex> Lock(Mutex);
	ConditionVariable.wait(Lock,
		[this]() -> bool
		{
			return this->TaskCount.load() == 0;
		}
	);
}
