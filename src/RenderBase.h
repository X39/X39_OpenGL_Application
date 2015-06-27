#pragma once

namespace X39
{
	namespace Entity
	{
		class RenderBase
		{
		public:
			virtual void render() = 0;
			virtual void loadRenderer() = 0;
			virtual void unloadRenderer() = 0;
		};
	}
}