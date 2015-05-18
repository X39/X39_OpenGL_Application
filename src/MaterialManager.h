#pragma once

#include "tgaLoader\tga.h"

#include <string>
#include <gl/gl.h>
#include <vector>

namespace X39
{
	namespace Singletons
	{
		#define TEXTURE_NA 0
		#define TEXTURE_DEFAULT 1
		#define TEXTURE_BASE 2
		#define TEXTURE_NORMAL 3

		typedef struct stTexture
		{
			std::string subname;
			std::string path;
			unsigned int type;
			long width;
			long height;
			long trueWidth;
			long trueHeight;
			unsigned char *data;
			long sizeOfDataArray;
			int internalFormat;
			int format;
			stTexture(){memset(this, 0, sizeof(struct stTexture));};
		} TEXTURE;
		typedef struct stMaterial
		{
			//tTGA textures[2];
			//GLuint gpuTextures[2];
			//bool hasTexture[2];
			std::vector<TEXTURE*> textures;
			std::vector<GLuint> gpuTextures;
			std::string vmatPath;
			std::vector<std::string> informationValue;
			std::vector<std::string> informationTitle;
		} MATERIAL;
		class MaterialManager
		{
			private:
				std::vector<MATERIAL*> matList;
				MaterialManager(void);

			public:
				~MaterialManager(void);
				static MaterialManager& getInstance(void);

				MATERIAL* registerTexture(const char* vmatPath){ return registerTexture(std::string(vmatPath)); }
				MATERIAL* registerTexture(std::string& vmatPath);
				void unregisterTexture(MATERIAL* mat);
				MATERIAL* createMaterial();
				
				void loadMaterial(MATERIAL* mat, unsigned int i = 0);
				void unloadMaterial(MATERIAL* mat, unsigned int i = 0);

				MATERIAL* getMaterialByVmatPath(char* vmatPath);
				MATERIAL* getMaterialByIndex(unsigned int i);
		};
	};
};
