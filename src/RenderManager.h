#pragma once
namespace X39
{
	namespace Singletons
	{
		class RenderManager
		{
		private:
			RenderManager();
			~RenderManager();
		public:
			static inline RenderManager& getInstance(void)
			{
				static RenderManager rm;
				return rm;
			}
			void prepare(void);
			void doRendering(void);
		};
	}
}