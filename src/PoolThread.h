#pragma once
#include "Simulation.h"
#include "BaseTask.h"
#include <thread>
namespace X39 {
	namespace Threading {
		class PoolThread
		{
		private:
			Simulation& sim;

			bool doTerminate;
			bool wasTerminated;
			bool waitForCheck;

			PoolThread(Simulation&);
			static void threadFunction(PoolThread* poolObject);
			~PoolThread();
		public:
			static PoolThread* createPoolThread(Simulation&);
			void terminate(bool);
			bool isTerminated(void);
		};

	}
}