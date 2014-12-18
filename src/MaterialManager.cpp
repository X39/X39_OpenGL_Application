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

		const MATERIAL* MaterialManager::registerTexture(char* vmatPath)
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
							mat->textureName = (const char*) opt.getValue();
						else if(strcmp(opt.getName(), "author") == 0)
							mat->textureAuthor = (const char*) opt.getValue();
						else if(strcmp(opt.getName(), "version") == 0)
							mat->textureVersion = (const char*) opt.getValue();
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

		void MaterialManager::loadMaterial(const MATERIAL* mat)
		{
			//ToDo: Load material into GPU if not yet done (GLuint array needs to be checked here)
			//ToDo: set material as current in GPU if already loaded into it
		}
		
		const MATERIAL* MaterialManager::getMaterialByVmatPath(const char* vmatPath)
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
		const MATERIAL* MaterialManager::getMaterialByIndex(unsigned int i)
		{
			if(i >= matList.size())
				return NULL;
			return matList[i];
		}

		void MaterialManager::reloadTextures(void)
		{
			while(this->matList.size() > 0)
			{
				unregisterTexture(this->matList[0]);
			}
			//TODO: Implement recursive material registration
		}
	}
}
