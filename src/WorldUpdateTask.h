#pragma once
#include "BaseTask.h"
#include "WorldBase.hpp"
#include "Simulation.h"
#include <vector>
namespace X39 {
	namespace Threading {
		class WorldUpdateTask : public BaseTask {
			::X39::WorldBase* world;
		public:
			WorldUpdateTask(::X39::WorldBase* wb) : world(wb) {}
			virtual void execute(void)
			{
				this->world->update();
			}
		};
	}
}