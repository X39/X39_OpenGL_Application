#include "globals.h"
#include "tgaLoader\tga.h"
#include "dotX39\SettingsDocument.h"
#include "dotX39\ISettingsFileHandler.h"
#include "CommandHandler\CommandHandler.h"

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
			if(sd == NULL)
			{
				LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("', cannot parse file"));
				return NULL;
			}
			SettingsNode root = *(sd->getRootNode());

			MATERIAL* mat = new MATERIAL();
			mat->vmatPath = std::string(vmatPath);
			std::string vmatDir = mat->vmatPath.substr(0, mat->vmatPath.find_last_of("/\\"));
			LOGGER_WRITE(::Logger::INFOImportant, std::string("Parsing vmat '").append(vmatPath).append("'"));
			for(unsigned int i = 0; i < root.childs.size(); i++)
			{
				SettingsNode node = *(root.childs[i]);
				if(strcmp(node.getName(), "textures") == 0)
				{
					for(unsigned int j = 0; j < node.childs.size(); j++)
					{
						SettingsNode textureNode = *(node.childs[j]);
						std::string relativePath;
						std::string type;
						for(unsigned int k = 0; k < textureNode.boundOptions.size(); k++)
						{
							SettingsOption opt = *(textureNode.boundOptions[k]);
							if(strcmp(opt.getName(), "relativePath") == 0)
								relativePath = (char*) opt.getValue();
							else if(strcmp(opt.getName(), "type") == 0)
								type = (char*) opt.getValue();
						}
						TEXTURE* texture = new TEXTURE();
						texture->subname = textureNode.getName();
						texture->type = CommandHandler::convAsciiCharToDouble(type.c_str(), -1);
						if(texture->type == -1)
						{
							LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("', cannot read type '").append(type).append("' of texture '").append(textureNode.getName()).append("', setting to TEXTURE_DEFAULT"));
							texture->type = TEXTURE_DEFAULT;
						}
						switch(texture->type)
						{
							case TEXTURE_NA:
								LOGGER_WRITE(::Logger::WARNING, std::string("\twhile parsing '").append(vmatPath).append("', found  TEXTURE_NA on texture '").append(textureNode.getName()).append("', skipping"));
								delete texture;
								continue;
							break;
							case TEXTURE_DEFAULT:
							case TEXTURE_BASE:
							case TEXTURE_NORMAL:
								LOGGER_WRITE(::Logger::INFO, std::string("\ttrying to read image file"));

								if(strEndsWith(relativePath, ".tga"))
								{
									tTGA tgaFile;
									if(load_TGA(&tgaFile, std::string(vmatDir).append("/").append(relativePath).c_str()) == FALSE)
									{
										LOGGER_WRITE(::Logger::ERRORmsg, std::string("\tfailed to load TGA '").append(vmatDir).append("/").append(relativePath).append("', skipping"));
										delete texture;
										continue;
									}
									texture->internalFormat = tgaFile.alpha ? 4 : 3;
									texture->format = tgaFile.alpha ? (GL_RGBA) : (GL_RGB);
									texture->data = tgaFile.data;
									texture->height = tgaFile.height;
									texture->sizeOfDataArray = tgaFile.sizeOfDataArray;
									texture->width = tgaFile.width;
								}
								else
								{
									LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("', texture '").append(textureNode.getName()).append("'has unknown file extension, skipping"));
									delete texture;
									continue;
								}
							break;
						}

						
						mat->textures.push_back(texture);
						mat->gpuTextures.push_back(0);
					}
				}
				else if(strcmp(node.getName(), "info") == 0)
				{
					for(unsigned int j = 0; j < node.boundOptions.size(); j++)
					{
						SettingsOption opt = *(node.boundOptions[j]);
						mat->informationValue.push_back(opt.getValue());
						mat->informationTitle.push_back(opt.getName());
					}
				}
				//for(unsigned int j = 0; j < node.boundOptions.size(); j++)
				//{
				//	SettingsOption opt = *(node.boundOptions[j]);
				//	if(strcmp(node.getName(), "textures") == 0)
				//	{
				//		if(strcmp(opt.getName(), "basetexture") == 0 && !mat->hasTexture[TEXTURE_BASETEXTURE])
				//		{
				//			//Prepare texture path
				//			char str[256];
				//			strcpy(str, vmatPath);
				//			char* tmpStr = strrchr(str, '\\');
				//			if(tmpStr == NULL)
				//			{
				//				LOGGER_WRITE(::Logger::ERRORmsg, "Failed to create path for texture:");
				//				LOGGER_WRITE(::Logger::TABULATOR, vmatPath);
				//				continue;
				//			}
				//			strcpy(tmpStr + 1, opt.getValue());
				//			//Actual loading
				//			if(load_TGA(&(mat->textures[TEXTURE_BASETEXTURE]), str) == FALSE)
				//			{
				//				mat->hasTexture[TEXTURE_BASETEXTURE] = false;
				//				LOGGER_WRITE(::Logger::ERRORmsg, "Failed to load tga:");
				//				LOGGER_WRITE(::Logger::TABULATOR, str);
				//				continue;
				//			}
				//			else
				//			{
				//				mat->hasTexture[TEXTURE_BASETEXTURE] = true;
				//				LOGGER_WRITE(::Logger::INFO, "loaded tga:");
				//				LOGGER_WRITE(::Logger::TABULATOR, str);
				//			}
				//		}
				//	}
				//	else if(strcmp(node.getName(), "info") == 0)
				//	{
				//		if(strcmp(opt.getName(), "name") == 0)
				//			mat->textureName = (char*) opt.getValue();
				//		else if(strcmp(opt.getName(), "author") == 0)
				//			mat->textureAuthor = (char*) opt.getValue();
				//		else if(strcmp(opt.getName(), "version") == 0)
				//			mat->textureVersion = (char*) opt.getValue();
				//	}
				//}
			}
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
					unloadMaterial(mat);
					LOGGER_WRITE(::Logger::INFOImportant, std::string("unloading texture '").append(mat->vmatPath).append("'"));
					m = matList[matList.size() - 1];
					matList[i] = m;
					matList[matList.size() - 1] = mat;
					for(unsigned int j = m->textures.size() - 1; j >= 0; j--)
					{
						TEXTURE* t = m->textures[j];
						
						//confirm that material got unloaded correctly from GPU
						GLuint id = m->gpuTextures[j];
						if(id != 0)
							LOGGER_WRITE(::Logger::ERRORmsg, std::string("Material '").append(mat->vmatPath).append("' still has a texture that is loaded to GPU RAM"));

						//Free texture on RAM lvl
						if (t->data)
							free(t->data);

						delete t;
					}
					//if(mat->hasTexture[TEXTURE_BASETEXTURE])
					//	free_TGA(&(mat->textures[TEXTURE_BASETEXTURE]));
					delete mat;
					matList.pop_back();
					break;
				}
			}
		}
		
		void MaterialManager::loadMaterial(MATERIAL* mat, unsigned int i)
		{
			if(mat->textures.size() <= i)
				return;
			if(mat->gpuTextures[i] == 0)
			{
				glEnable( GL_TEXTURE_2D );
				glGenTextures( 1, &(mat->gpuTextures[i]) );

				
				float maxAni;
				glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAni );                                              
				glBindTexture(GL_TEXTURE_2D,  (mat->gpuTextures[i]));
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
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					mat->textures[i]->internalFormat,
					mat->textures[i]->width,
					mat->textures[i]->height,
					0,
					mat->textures[i]->format,
					GL_UNSIGNED_BYTE,
					mat->textures[i]->data
				);
			}
			else
			{
				float maxAni;
				glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAni );                                              
				glBindTexture(GL_TEXTURE_2D,  (mat->gpuTextures[i]));
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
		void MaterialManager::unloadMaterial(MATERIAL* mat, unsigned int i)
		{
			if(mat->textures.size() <= i)
				return;
			if(mat->gpuTextures[i] != 0)
			{
				glDeleteTextures(1, &(mat->gpuTextures[i]));
				mat->gpuTextures[i] = 0;
			}
		}
		MATERIAL* MaterialManager::createMaterial()
		{
			MATERIAL* m = new MATERIAL();
			this->matList.push_back(m);
			return m;
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
