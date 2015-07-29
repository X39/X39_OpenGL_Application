#pragma once
#include "BaseTask.h"
#include "EntityBase.h"
#include <vector>
namespace X39 {
	class Simulation final
	{
	private:
		void prepareThreadPool();
		Simulation()
		{
		}
		~Simulation()
		{
		}
	public:
		static inline Simulation& getInstance()
		{
			static Simulation instance;
			return instance;
		}
		void doRendering();

		Threading::BaseTask* getNextTask(void*);
		void addTask(Threading::BaseTask*);
		void setTaskAsDone(Threading::BaseTask*);
		void addEntity(::X39::Entity::EntityBase* ent);
		void removeEntity(::X39::Entity::EntityBase* ent);
		const std::vector<::X39::Entity::EntityBase*>& getEntityList(void);
		void Simulation::performEntityDrop(void);
		void init(void);
		void uninit(void);
	};
}