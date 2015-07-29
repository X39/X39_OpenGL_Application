#pragma once

namespace X39
{
	namespace Entity
	{
		class RenderBase
		{
		private:
			bool _isLoaded;
		public:
			RenderBase();
			~RenderBase();
			virtual void doRender() final;
			virtual void render() = 0;
			virtual void loadRenderer() {};
			virtual void unloadRenderer() {};
		};
	}
}