#pragma once
#include "globals.h"
#include "glHeaders.h"

#include "EntityBase.h"

#include <vector>



namespace X39
{
	namespace EntitySystem
	{
		class EntityFactory
		{
		private:
			int maxEntitiesPerWorker;
			std::vector<void*> workers;
		protected:
			std::vector<void*> entityList;
		public:
			EntityFactory(void);
			EntityFactory(int i);
			~EntityFactory(void);
			void InitEntitySystem(void);
			void TerminateAllEntities(void);
			void setMaxEntitiesPerWorker(int val);
			EntityBase* getEntity(int i);
			void createEntity(EntityBase* ent, void* world);
		};
	};
};