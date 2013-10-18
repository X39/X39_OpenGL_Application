#include "EntityBase.h"



namespace X39
{
	namespace EntitySystem
	{
		EntityBase::EntityBase(void)
		{
			ID;
			entityPosition;
			entityDirection;
		}

		EntityBase::~EntityBase(void)
		{
		}


		void EntityBase::updateTick(void)
		{

		}
	};
};