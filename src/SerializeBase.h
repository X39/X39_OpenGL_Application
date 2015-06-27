#pragma once

namespace X39
{
	namespace Entity
	{
		class SerializeBase
		{
		public:
			virtual void Serialize() = 0;
			virtual void Unserialize() = 0;
		};
	}
}