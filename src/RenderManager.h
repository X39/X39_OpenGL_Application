#pragma once
#include "RenderBase.h"
#include "WorldBase.h"
#include <vector>
namespace X39
{
	namespace Singletons
	{
		class RenderManager
		{
		private:
			RenderManager();
			~RenderManager();
			std::vector<::X39::Entity::RenderBase*> _entityList;
		public:
			static inline RenderManager& getInstance(void)
			{
				static RenderManager rm;
				return rm;
			}
			void prepare(void);
			void doRendering(void);
			void addEntity(::X39::Entity::RenderBase* ent);
			void removeEntity(::X39::Entity::RenderBase* ent);
			void setWorld(::X39::WorldBase*);
		};
	}
}