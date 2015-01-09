#pragma once

#include "tgaLoader\tga.h"

#include <string>
#include <gl/gl.h>
#include <vector>

namespace X39
{
	namespace Singletons
	{
		#define TEXTURE_BASETEXTURE 0

		typedef struct stMaterial
		{
			tTGA textures[2];
			GLuint gpuTextures[2];
			bool hasTexture[2];
			std::string vmatPath;
			std::string textureName;
			std::string textureAuthor;
			std::string textureVersion;
			stMaterial(){memset(this, 0, sizeof(struct stMaterial));memset(this->gpuTextures, ~0, sizeof(gpuTextures));};
		} MATERIAL;
		class MaterialManager
		{
			private:
				std::vector<MATERIAL*> matList;
				MaterialManager(void);

			public:
				~MaterialManager(void);
				static MaterialManager& getInstance(void);

				MATERIAL* registerTexture(char* vmatPath);
				void unregisterTexture(MATERIAL* mat);
				
				void loadMaterial(MATERIAL* mat);
				void unloadMaterial(MATERIAL* mat);

				MATERIAL* getMaterialByVmatPath(char* vmatPath);
				MATERIAL* getMaterialByIndex(unsigned int i);
		};
	}
}
