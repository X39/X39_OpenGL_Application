#include "Simulation.h"
#include "PoolThread.h"
#include "BaseTask.h"
#include "RenderManager.h"

#include <vector>
#include <queue>
#include <mutex>
#include <omp.h>


namespace X39 {

	std::vector<Threading::PoolThread*> threadPool;
	std::vector<Entity::EntityBase*> _entityList;
	std::vector<Entity::EntityBase*> _dropList;
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
			it->terminate(true);
		for (auto& it : threadPool)
		{
			while (!it->isTerminated());
			it->terminate(false);
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
		//for (int i = 0; i < 10; i++)
			threadPool.push_back(Threading::PoolThread::createPoolThread(*this));
	}

	Threading::BaseTask* Simulation::getNextTask(void* thread)
	{
		taskMutex.lock();
		Threading::BaseTask* returnValue;
		if (toDoTaskList.empty())
		{
			if (threadPool.size() > 1)
				((Threading::PoolThread*)thread)->terminate(false,
				[](Threading::PoolThread* pt) {
				taskMutex.lock();
				for (int i = 0; i < threadPool.size(); i++)
				{
					auto it = threadPool[i];
					if (it == pt)
					{
						threadPool[i] = threadPool.back();
						threadPool.pop_back();
						break;
					}
				}
				taskMutex.unlock();
			});
			returnValue = NULL;
		}
		else
		{
			size_t taskListSize = toDoTaskList.size();
			if (taskListSize / 100 / threadPool.size() > 0)
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
	void Simulation::addEntity(Entity::EntityBase* ent)
	{
		try
		{
			taskMutex.lock();
			_entityList.push_back(ent);
			taskMutex.unlock();
		}
		catch (std::exception e)
		{
			LOGGER_WRITE(Logger::ERRORmsg, e.what());
		}
	}
	void Simulation::performEntityDrop(void)
	{
		taskMutex.lock();
		for (int i = 0; i < _entityList.size(); i++)
		{
			for (int j = 0; j < _dropList.size(); j++)
			{
				if (_entityList[i] == _dropList[j])
				{
					_entityList[i] = _entityList.back();
					_entityList.pop_back();
					_dropList[j]->deconstructEntity();
					_dropList[j] = _dropList.back();
					_dropList.pop_back();
					break;
				}
			}
		}
		taskMutex.unlock();
	}
	void Simulation::removeEntity(Entity::EntityBase* ent)
	{
		_dropList.push_back(ent);
	}
	const std::vector<::X39::Entity::EntityBase*>& Simulation::getEntityList(void)
	{
		return _entityList;
	}
	void Simulation::setWorld(::X39::WorldBase* world)
	{
		taskMutex.lock();
		if (currentWorld != nullptr)
		{
			currentWorld->destroyWorld();
			delete currentWorld;
		}
		currentWorld = world;
		for (int i = 0; i < toDoTaskList.size(); i++)
		{
			delete toDoTaskList.front();
			toDoTaskList.pop();
		}
		taskMutex.unlock();
		Singletons::RenderManager::getInstance().setWorld(currentWorld);
		if (currentWorld != nullptr)
		{
			currentWorld->prepareWorld();
		}
	}
	::X39::WorldBase* Simulation::getWorld(void)
	{
		return currentWorld;
	}
	unsigned int Simulation::getThreadCount(void)
	{
		return threadPool.size();
	}
}