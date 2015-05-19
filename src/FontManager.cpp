#include "globals.h"
#include "FontManager.h"
#include "Logger.h"
#include <Windows.h>
#include <string>


namespace X39
{
	namespace Singletons
	{
		FontManager::FontManager(void)
		{
			FT_Error err = FT_Init_FreeType(&library);
			if(err != FT_Err_Ok)
			{
				FatalAppExit(NULL, TEXT("Cannot initialize FontManager!"));
			}
			fontShader = ::X39::Shader();
			fontShader.load("Shaders\\font.shad");
			fontShader.compile();
		}

		FontManager::~FontManager(void)
		{
		}

		FONT* FontManager::registerFont(std::string path)
		{
			FT_Error err;
			FT_Face face;
			//Load font file into face
			err = FT_New_Face(this->library, path.c_str(), 0, &face);
			if(err != FT_Err_Ok)
			{
				LOGGER_WRITE(Logger::ERRORmsg, std::string("Cannot load font file '").append(path).append("' for some reason"));
				return NULL;
			}
			FONT* font = new FONT();
			font->face = face;
			font->material = ::X39::Singletons::MaterialManager::getInstance().createMaterial();

			this->fontList.push_back(font);
			return font;
		}

		unsigned int FontManager::getCharTextureIndex(FONT* font, char c)
		{
			if(font->material->textures.size() <= c)
			{
				font->material->textures.resize(c + 1);
				font->material->gpuTextures.resize(c + 1);
			}
			TEXTURE* texture = font->material->textures[c];
			if(texture == 0x00)
			{
				FT_Error		err;
				//err = FT_Set_Char_Size(font->face, 256, 0, 100, 0);
				err = FT_Set_Pixel_Sizes(font->face, 256, 256);
				if(err != FT_Err_Ok)
				{
					char stringWorkaround[2] = {c, 0x00};
					LOGGER_WRITE(Logger::WARNING, std::string("cannot generate index for char '").append(stringWorkaround).append("'"));
					return 0;
				}
				err = FT_Load_Char(font->face, c, FT_LOAD_RENDER);
				if(err != FT_Err_Ok)
				{
					char stringWorkaround[2] = {c, 0x00};
					LOGGER_WRITE(Logger::WARNING, std::string("cannot generate index for char '").append(stringWorkaround).append("'"));
					return 0;
				}
				texture = new TEXTURE();
				texture->textureUnit = TEXTUREUNITFONTS;
				font->material->textures[c] = texture;

				texture->trueHeight = font->face->glyph->bitmap.rows;
				texture->trueWidth = font->face->glyph->bitmap.width;
				texture->subname = c;
				unsigned int largerSide = (texture->trueHeight > texture->trueWidth ? texture->trueHeight : texture->trueWidth);
				unsigned int powerOf2 = 1;
				while(largerSide > powerOf2)
					powerOf2 <<= 1;
				texture->data = new unsigned char[powerOf2 * powerOf2 * 4];
				for(int i = 0; i < powerOf2; i++)
					for(int j = 0; j < powerOf2; j++)
						texture->data[i * powerOf2 + j] = (i >= texture->trueHeight || j >= texture->trueWidth ? 0 : font->face->glyph->bitmap.buffer[i * texture->trueWidth + j]);
				
				texture->sizeOfDataArray = powerOf2 * powerOf2;
				texture->height	= powerOf2;
				texture->width	= powerOf2;
				texture->internalFormat = 1;
				texture->format = GL_RED;
			}
			return c;
		}

		FONT* FontManager::getFont(unsigned int index)
		{
			if(index < 0 || index > this->fontList.size())
				return NULL;
			return this->fontList[index];
		}

		FontManager& FontManager::getInstance(void)
		{
			static FontManager instance;
			return instance;
		}
	};
};