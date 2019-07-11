#pragma once
#include "Simulation.h"
#include "BaseTask.h"
#include <thread>
#include <functional>
namespace X39 {
	namespace Threading {
		class PoolThread
		{
		private:
			Simulation& sim;

			bool doTerminate;
			bool wasTerminated;
			bool waitForCheck;
			std::function<void(PoolThread*)> callback;

			PoolThread(Simulation&);
			static void threadFunction(PoolThread* poolObject);
			~PoolThread();
		public:
			static PoolThread* createPoolThread(Simulation&);
			void terminate(bool, std::function<void(PoolThread*)> = std::function<void(PoolThread*)>());
			bool isTerminated(void);
		};

	}
}