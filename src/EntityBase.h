#pragma once

namespace X39
{
	namespace Entity
	{
		class EntityBase
		{
		protected:
			~EntityBase() {};
		public:
			EntityBase();
			virtual void deconstructEntity() { delete this; }
			virtual void kill() final;
			virtual void update() = 0;
			virtual bool isVisible() = 0;
			virtual void onKill() {};
		};
	}
}