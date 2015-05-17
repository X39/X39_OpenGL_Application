#include "globals.h"
#include "tgaLoader\tga.h"
#include <dotX39\DocumentReader.h>
#include <dotX39\DataString.h>
#include <dotX39\DataScalar.h>
#include "CommandHandler\CommandHandler.h"

#include <vector>
#include <string>

#include "MaterialManager.h"


using namespace dotX39;
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

		MATERIAL* MaterialManager::registerTexture(std::string& vmatPath)
		{
			Node* root;
			try
			{
				root = new Node("root");
				DocumentReader::readDocument(vmatPath.c_str(), root);
			}
			catch (std::exception ex)
			{
				LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("': ").append(ex.what));
				delete root;
				return false;
			}

			MATERIAL* mat = new MATERIAL();
			mat->vmatPath = std::string(vmatPath);
			std::string vmatDir = mat->vmatPath.substr(0, mat->vmatPath.find_last_of("/\\"));
			LOGGER_WRITE(::Logger::INFOImportant, std::string("Parsing vmat '").append(vmatPath).append("'"));
			for (unsigned int rootNodeIndex = 0; rootNodeIndex < root->getNodeCount(); rootNodeIndex++)
			{
				const Node* layer1 = root->getNode(rootNodeIndex);
				//SettingsNode node = *(root.childs[i]);
				if (layer1->getName().compare("textures") == 0)
				{
					for (unsigned int layer1NodeIndex = 0; layer1NodeIndex < layer1->getDataCount(); layer1NodeIndex++)
					{
						const Node* layer2 = layer1->getNode(layer1NodeIndex);
						TEXTURE* texture = new TEXTURE();
						texture->subname = layer2->getName();
						for (unsigned int layer2ArgumentIndex = 0; layer2ArgumentIndex < layer2->getArgumentCount(); layer2ArgumentIndex++)
						{
							const Data* argument = layer2->getArgument(layer2ArgumentIndex);
							if (argument->getName().compare("path") == 0)
							{
								if (argument->getType() != DataTypes::STRING)
								{
									LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("', '").append(layer1->getName()).append("/").append(layer2->getName()).append("' has invalid type for '").append(argument->getName()).append("'! Expected STRING."));
									delete root;
									delete texture;
									return false;
								}
								texture->path = std::string(vmatDir).append("/").append(((DataString*)argument)->getDataAsString());
							}
							else if (argument->getName().compare("type") == 0)
							{
								if (argument->getType() != DataTypes::SCALAR)
								{
									LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(vmatPath).append("', '").append(layer1->getName()).append("/").append(layer2->getName()).append("' has invalid type for '").append(argument->getName()).append("'! Expected SCALAR."));
									delete root;
									delete texture;
									return false;
								}
								texture->type = (unsigned int)((DataScalar*)argument)->getDataAsLongDouble();
							}
						}
						if (texture->type == TEXTURE_NA)
						{
							LOGGER_WRITE(::Logger::WARNING, std::string("\tType TEXTURE_NA(").append(STRINGIFY(TEXTURE_NA)).append(") for texture '").append(layer2->getName()).append("' is invalid, setting to TEXTURE_DEFAULT(").append(STRINGIFY(TEXTURE_DEFAULT)).append(")"));
							texture->type = TEXTURE_DEFAULT;
						}
						switch(texture->type)
						{
							case TEXTURE_DEFAULT: case TEXTURE_BASE: case TEXTURE_NORMAL:
								LOGGER_WRITE(::Logger::DEBUG, std::string("\ttrying to read image file"));

								if (strEndsWith(texture->path, ".tga"))
								{
									tTGA tgaFile;
									if (load_TGA(&tgaFile, texture->path.c_str()) == FALSE)
									{
										LOGGER_WRITE(::Logger::ERRORmsg, std::string("\tfailed to load TGA '").append(texture->path).append("', skipping"));
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
									LOGGER_WRITE(::Logger::WARNING, std::string("\tWhile parsing '").append(vmatPath).append("', texture '").append(layer2->getName()).append("'has unknown file extension, skipping"));
									delete texture;
									continue;
								}
							break;
						}
						mat->textures.push_back(texture);
						mat->gpuTextures.push_back(0);
					}
				}
				else if (layer1->getName().compare("info") == 0)
				{
					for (unsigned int layer1DataIndex = 0; layer1DataIndex < layer1->getDataCount(); layer1DataIndex++)
					{
						const Data* data = layer1->getData(layer1DataIndex);
						switch (data->getType())
						{
						case DataTypes::STRING:
								mat->informationValue.push_back(((DataString*)data)->getDataAsString());
								mat->informationTitle.push_back(data->getName());
								LOGGER_WRITE(::Logger::DEBUG, std::string("\t").append("Added '").append(layer1->getName()).append("/").append(data->getName()).append("' with content '").append(((DataString*)data)->getDataAsString()).append("' to MATERIAL info array"));
								break;
						default:
							LOGGER_WRITE(::Logger::WARNING, std::string("\t'").append(vmatPath).append("' hit unexpected datatype on '").append(layer1->getName()).append("/").append(data->getName()).append("'"));
							break;
						}
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
			delete root;
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
