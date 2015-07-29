#pragma once
#include "BaseTask.h"
#include "EntityBase.h"
#include "Simulation.h"
#include <vector>
namespace X39 {
	namespace Threading {
		class EntityUpdateTask : public BaseTask {
			std::vector<Entity::EntityBase*> list;
			size_t firstIndex;
			size_t lastIndex;
		public:
			EntityUpdateTask(std::vector<Entity::EntityBase*> v, size_t fIndex, size_t lIndex)
			{
				list = v;
				firstIndex = fIndex;
				lastIndex = lIndex;
			}
			virtual void execute(void)
			{
				for (int i = this->firstIndex; i < this->lastIndex; i++)
				{
					auto& it = this->list[i];
					it->update();
				}
			}
		};
	}
}