#include "PoolThread.h"

namespace X39 {
	namespace Threading {
		PoolThread::PoolThread(Simulation& simulationRef) : sim(simulationRef), doTerminate(false), wasTerminated(false), waitForCheck(false)
		{

		}

		PoolThread::~PoolThread()
		{
			while (!this->isTerminated());
		}

		PoolThread* PoolThread::createPoolThread(Simulation& simulation)
		{
			PoolThread* poolThread = new PoolThread(simulation);
			std::thread thread = std::thread(threadFunction, poolThread);
			thread.detach();
			return poolThread;
		}
		void PoolThread::threadFunction(PoolThread* poolObject)
		{
			while (!poolObject->doTerminate)
			{
				BaseTask* task = poolObject->sim.getNextTask(poolObject);
				if (task != NULL)
				{
					task->execute();
					poolObject->sim.setTaskAsDone(task);
				}
			}
			poolObject->wasTerminated = true;
			while (poolObject->waitForCheck);
			delete poolObject;
		}
		void PoolThread::terminate(bool waitForCheck)
		{
			this->doTerminate = true;
			this->waitForCheck = waitForCheck;
		}
		bool PoolThread::isTerminated(void)
		{
			return this->wasTerminated;
		}
	}
}