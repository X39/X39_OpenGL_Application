#include "EntityFactoryWorker.h"


namespace X39
{
	namespace EntitySystem
	{
		EntityFactoryWorker::EntityFactoryWorker(void):EntityFactory()
		{
		}

		EntityFactoryWorker::~EntityFactoryWorker(void)
		{
		}
		
		void* runThread(void* args)
		{
			return 0;
		}
		int getLivingEntityCount()
		{
			return -1;
		}
	};
};