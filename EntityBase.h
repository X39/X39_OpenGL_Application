#pragma once
#include "globals.h"
#include "glHeaders.h"

namespace X39
{
	namespace EntitySystem
	{
		class EntityBase
		{
		protected:
			glm::vec3 entityPosition;
			glm::vec4 entityDirection;
	
		public:
			long int ID;
			EntityBase(void);
			~EntityBase(void);

			virtual void updateTick(void);
		};
	};
};