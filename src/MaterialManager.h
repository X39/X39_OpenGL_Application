#pragma once
#include "globals.h"
#include "tgaLoader\tga.h"
#include "dotX39\SettingsDocument.h"
#include "dotX39\ISettingsFileHandler.h"

#include <vector>
#include <string>
namespace X39
{
	namespace Singletons
	{
		#define TEXTURE_BASETEXTURE 0

		typedef struct stMaterial
		{
			tTGA textures[1];
			GLuint textures[1];
			bool hasTexture[1];
			std::string vmatPath;
			std::string textureName;
			std::string textureAuthor;
			std::string textureVersion;
			stMaterial(){memset(this, 0, sizeof(struct stMaterial));};
		} MATERIAL;
		class MaterialManager
		{
			private:
				std::vector<MATERIAL*> matList;
				MaterialManager(void);

			public:
				~MaterialManager(void);
				static MaterialManager& getInstance(void);

				const MATERIAL* registerTexture(char* vmatPath);
				void unregisterTexture(MATERIAL* mat);

				void loadMaterial(const MATERIAL* mat);

				const MATERIAL* getMaterialByVmatPath(const char* vmatPath);
				const MATERIAL* getMaterialByIndex(unsigned int i);

				void reloadTextures(void);
		};
	}
}
