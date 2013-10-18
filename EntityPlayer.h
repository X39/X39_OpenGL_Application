#pragma once
#include "EntityBase.h"

#include "KeyHandler.h"
#include "KeyDefines.h"




namespace X39
{
	namespace EntitySystem
	{
		class EntityPlayer : public EntityBase
		{
			friend class EntityBase;
		public:
			EntityPlayer(void);
			~EntityPlayer(void);
			virtual void updateTick(void);
		};
	};
};