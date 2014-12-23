#include "globals.h"
#include "tgaLoader\tga.h"
#include "dotX39\SettingsDocument.h"
#include "dotX39\ISettingsFileHandler.h"

#include <vector>
#include <string>

#include "MaterialManager.h"

namespace X39
{
	namespace Singletons
	{
		MaterialManager::MaterialManager(void)
		{
		}

		MaterialManager::~MaterialManager(void)
		{
		}

		MaterialManager& MaterialManager::getInstance(void)
		{
			static MaterialManager instance;
			return instance;
		}

		MATERIAL* MaterialManager::registerTexture(char* vmatPath)
		{
			ISettingsFileHandler fh = ISettingsFileHandler(vmatPath);
			SettingsDocument* sd = fh.load();
			if(sd == NULL) //TODO: Add error message
				return NULL;
			SettingsNode root = *(sd->getRootNode());

			MATERIAL* mat = new MATERIAL();
			mat->vmatPath = std::string(vmatPath);
			for(unsigned int i = 0; i < root.childs.size(); i++)
			{
				SettingsNode node = *(root.childs[i]);
				for(unsigned int j = 0; j < node.boundOptions.size(); j++)
				{
					SettingsOption opt = *(node.boundOptions[j]);
					if(strcmp(node.getName(), "textures") == 0)
					{
						if(strcmp(opt.getName(), "basetexture") == 0 && !mat->hasTexture[TEXTURE_BASETEXTURE])
						{
							//Prepare texture path
							char str[256];
							strcpy(str, vmatPath);
							char* tmpStr = strrchr(str, '\\');
							if(tmpStr == NULL)
							{
								LOGGER_WRITE(::Logger::ERRORmsg, "Failed to create path for texture:");
								LOGGER_WRITE(::Logger::TABULATOR, vmatPath);
								continue;
							}
							strcpy(tmpStr + 1, opt.getValue());
							//Actual loading
							if(load_TGA(&(mat->textures[TEXTURE_BASETEXTURE]), str) == FALSE)
							{
								mat->hasTexture[TEXTURE_BASETEXTURE] = false;
								LOGGER_WRITE(::Logger::ERRORmsg, "Failed to load tga:");
								LOGGER_WRITE(::Logger::TABULATOR, str);
								continue;
							}
							else
							{
								mat->hasTexture[TEXTURE_BASETEXTURE] = true;
								LOGGER_WRITE(::Logger::INFO, "loaded tga:");
								LOGGER_WRITE(::Logger::TABULATOR, str);
							}
						}
					}
					else if(strcmp(node.getName(), "info") == 0)
					{
						if(strcmp(opt.getName(), "name") == 0)
							mat->textureName = (char*) opt.getValue();
						else if(strcmp(opt.getName(), "author") == 0)
							mat->textureAuthor = (char*) opt.getValue();
						else if(strcmp(opt.getName(), "version") == 0)
							mat->textureVersion = (char*) opt.getValue();
					}
				}
			}
			LOGGER_WRITE(::Logger::INFO, "loaded texture:");
			LOGGER_WRITE(::Logger::TABULATOR, vmatPath);
			this->matList.push_back(mat);
			delete sd;
			return mat;
		}

		void MaterialManager::unregisterTexture(MATERIAL* mat)
		{
			for(unsigned int i = 0; i < matList.size(); i++)
			{
				MATERIAL* m = matList[i];
				if(m == mat)
				{
					LOGGER_WRITE(::Logger::INFO, "unloading texture:");
					LOGGER_WRITE(::Logger::INFO, mat->vmatPath.c_str());
					m = matList[matList.size() - 1];
					matList[i] = m;
					matList[matList.size() - 1] = mat;
					if(mat->hasTexture[TEXTURE_BASETEXTURE])
						free_TGA(&(mat->textures[TEXTURE_BASETEXTURE]));
					delete mat;
					matList.pop_back();
					break;
				}
			}
		}
		
		void MaterialManager::loadMaterial(MATERIAL* mat)
		{
			if(mat->gpuTextures[TEXTURE_BASETEXTURE] == ~0)
			{
				glEnable( GL_TEXTURE_2D );
				glGenTextures( 1, &(mat->gpuTextures[TEXTURE_BASETEXTURE]) );

				
				float maxAni;
				glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAni );                                              
				glBindTexture(GL_TEXTURE_2D,  (mat->gpuTextures[TEXTURE_BASETEXTURE]));
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAni );   
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				/*
				 *	possible values:
				 *	- GL_NEAREST
				 *	- GL_LINEAR
				 *	- GL_NEAREST_MIPMAP_NEAREST
				 *	- GL_NEAREST_MIPMAP_LINEAR
				 *	- GL_LINEAR_MIPMAP_NEAREST
				 *	- GL_LINEAR_MIPMAP_LINEAR
				 */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);

			#ifdef __GLU_H__
				gluBuild2DMipmaps(
					GL_TEXTURE_2D,
					((mat->textures[TEXTURE_BASETEXTURE]).alpha) ? 4 : 3,
					(mat->textures[TEXTURE_BASETEXTURE]).width,
					(mat->textures[TEXTURE_BASETEXTURE]).height,
					((mat->textures[TEXTURE_BASETEXTURE]).alpha) ? (GL_RGBA) : (GL_RGB),
					GL_UNSIGNED_BYTE,
					(mat->textures[TEXTURE_BASETEXTURE]).data
				);
			#else
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					(tga->alpha) ? 4 : 3,
					tga->width,
					tga->height,
					0,
					(tga->alpha) ? (GL_RGBA) : (GL_RGB),
					GL_UNSIGNED_BYTE,
					tga->data
				);
			#endif
			}
			else
			{
				float maxAni;
				glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAni );                                              
				glBindTexture(GL_TEXTURE_2D,  (mat->gpuTextures[TEXTURE_BASETEXTURE]));
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAni );   
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				/*
				 *	possible values:
				 *	- GL_NEAREST
				 *	- GL_LINEAR
				 *	- GL_NEAREST_MIPMAP_NEAREST
				 *	- GL_NEAREST_MIPMAP_LINEAR
				 *	- GL_LINEAR_MIPMAP_NEAREST
				 *	- GL_LINEAR_MIPMAP_LINEAR
				 */
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
		}
		void MaterialManager::unloadMaterial(MATERIAL* mat)
		{
			if(mat->gpuTextures[TEXTURE_BASETEXTURE] != ~0)
			{
				glDeleteTextures(1, &(mat->gpuTextures[TEXTURE_BASETEXTURE]));
				mat->gpuTextures[TEXTURE_BASETEXTURE] = ~0;
			}
		}
		
		MATERIAL* MaterialManager::getMaterialByVmatPath(char* vmatPath)
		{
			for(unsigned int i = 0; i < matList.size(); i++)
			{
				MATERIAL* m = matList[i];
				if(m->vmatPath.compare(vmatPath))
				{
					return m;
				}
			}
			return NULL;
		}
		MATERIAL* MaterialManager::getMaterialByIndex(unsigned int i)
		{
			if(i >= matList.size())
				return NULL;
			return matList[i];
		}
	}
}
