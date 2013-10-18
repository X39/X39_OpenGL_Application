#include "EntityFactory.h"

#include "EntityFactoryWorker.h"


namespace X39
{
	namespace EntitySystem
	{
		EntityFactory::EntityFactory(void)
		{
			entityList = std::vector<void*>();
		}
		EntityFactory::EntityFactory(int i)
		{
			maxEntitiesPerWorker = i;
			workers = std::vector<void*>();
			entityList = std::vector<void*>();
		}
		EntityFactory::~EntityFactory(void)
		{
			for(unsigned int i = 0; i < workers.size(); i++)
			{
				delete workers[i];
			}
			workers.clear();
		}
		
		void EntityFactory::InitEntitySystem(void)
		{
			for(unsigned int i = 0; i < entityList.size(); i++)
			{
				((EntityBase*)entityList[i])->updateTick();
			}
		}
		void EntityFactory::TerminateAllEntities(void)
		{
		}
		void EntityFactory::setMaxEntitiesPerWorker(int val)
		{
		}
		EntityBase* EntityFactory::getEntity(int i)
		{
			return NULL;
		}
		void EntityFactory::createEntity(EntityBase* ent, void* world)
		{
			ent->ID = 0; //TODO
			entityList.push_back(ent);
		}
	};
};