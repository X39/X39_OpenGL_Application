#include "Simulation.h"
#include "PoolThread.h"
#include "BaseTask.h"
#include "RenderManager.h"

#include <vector>
#include <queue>
#include <mutex>


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
	void Simulation::addEntity(Entity::EntityBase* ent)
	{
		_entityList.push_back(ent);
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
					_dropList[i] = _dropList.back();
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
		if (currentWorld != nullptr)
		{
			currentWorld->destroyWorld();
			delete currentWorld;
		}
		currentWorld = world;
		if (currentWorld != nullptr)
		{
			currentWorld->prepareWorld();
		}
	}
}