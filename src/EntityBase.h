#pragma once

namespace X39
{
	namespace Entity
	{
		class EntityBase
		{
		public:
			EntityBase();
			~EntityBase();
			virtual void kill() final;
			virtual void update() = 0;
			virtual bool isVisible() = 0;
			virtual void onKill() {};
		};
	}
}