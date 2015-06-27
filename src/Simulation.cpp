#include "Simulation.h"
#include "PoolThread.h"
#include "BaseTask.h"
#include "RenderManager.h"

#include <vector>
#include <queue>
#include <mutex>


namespace X39 {

	std::vector<Threading::PoolThread*> threadPool;
	std::queue<Threading::BaseTask*> toDoTaskList;
	std::mutex taskMutex;

	void Simulation::init(void)
	{
		Singletons::RenderManager::getInstance().prepare();
		this->prepareThreadPool();
	}
	void Simulation::uninit(void)
	{
		for (auto& it : threadPool)
		{
			it->terminate(true);
			while (!it->isTerminated());
		}
		threadPool.clear();
		while (!toDoTaskList.empty())
		{
			auto& it = toDoTaskList.front();
			delete it;
			toDoTaskList.pop();
		}
	}
	void Simulation::doRendering()
	{
		Singletons::RenderManager::getInstance().doRendering();
	}
	void Simulation::prepareThreadPool()
	{
		threadPool.push_back(Threading::PoolThread::createPoolThread(*this));
	}

	Threading::BaseTask* Simulation::getNextTask(void* thread)
	{
		taskMutex.lock();
		Threading::BaseTask* returnValue;
		if (toDoTaskList.empty())
		{
			if (threadPool.size() > 1)
				((Threading::PoolThread*)thread)->terminate(false);
			returnValue = NULL;
		}
		else
		{
			size_t taskListSize = toDoTaskList.size();
			if (taskListSize / 100 > 0)
			{
				threadPool.push_back(Threading::PoolThread::createPoolThread(*this));
			}
			returnValue = toDoTaskList.front();
			toDoTaskList.pop();
		}
		taskMutex.unlock();
		return returnValue;
	}
	void Simulation::addTask(Threading::BaseTask* task)
	{
		taskMutex.lock();
		toDoTaskList.push(task);
		taskMutex.unlock();
	}
	void Simulation::setTaskAsDone(Threading::BaseTask* task)
	{
		delete task;
	}
}