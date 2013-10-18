#pragma once
#include "EntityFactory.h"


namespace X39
{
	namespace EntitySystem
	{
		class EntityFactoryWorker : public EntityFactory
		{
		private:
			int pID;
			void workerProcess(void* args);

		public:
			EntityFactoryWorker(void);
			~EntityFactoryWorker(void);

			void* runThread(void* args);
			int getLivingEntityCount();
		};
	};
};