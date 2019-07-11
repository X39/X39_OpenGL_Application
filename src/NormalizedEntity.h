#pragma once
#include "globals.h"
#include "EntityLiving.h"
#include "RenderBase.h"
namespace X39
{
	namespace Entity
	{
		class NormalizedEntity : public EntityLiving, public RenderBase
		{
		protected:
			GLuint test_vaoID;
			GLuint test_vertBufferID;
			GLuint test_indexBufferID;
			::X39::Shader* test_shad;
			~NormalizedEntity()
			{
				this->deconstructRenderer();
			}
		public:
			NormalizedEntity(double i) : EntityLiving(0, 1, i, 0.001 * i), RenderBase() {}
			NormalizedEntity(double i, double j, double k) : EntityLiving(i * 2, j * 2, k * 2, 0.005 * i, 0.005 * j, 0.005 * k), RenderBase() {}
			virtual void deconstructEntity();
			virtual void render();
			virtual void loadRenderer();
			virtual void unloadRenderer();
			virtual bool isVisible();
			virtual void onUpdate();
		};
	}
}