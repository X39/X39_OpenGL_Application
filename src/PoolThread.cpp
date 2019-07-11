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
			if (poolObject->callback)
				poolObject->callback(poolObject);
			delete poolObject;
		}
		void PoolThread::terminate(bool waitForCheck, std::function<void(PoolThread*)> callback)
		{
			this->doTerminate = true;
			this->waitForCheck = waitForCheck;
			this->callback = callback;
		}
		bool PoolThread::isTerminated(void)
		{
			return this->wasTerminated;
		}
	}
}