#include "globals.h"
#include "FontManager.h"
#include "Logger.h"
#include <Windows.h>
#include <string>
#include <cctype>
#include <fstream>


namespace X39
{
	namespace Singletons
	{
		/*
		 * BIG TODO:
		 *	Change entire font rendering from pre-defined alwaysTheSame font rendering to dynamic font rendering
		 *	(currently all is clamped to 512 pixels x 512 pixels thus the advance is always the same)
		 */
		static void exportToPGM(unsigned char* data, size_t width, size_t height, const char* filePath)
		{
			std::ofstream stream = std::ofstream();
			stream.open(filePath, std::ios_base::binary);
			stream << "P5" << std::endl;
			stream << width << " " << height << std::endl;
			stream << 255 << std::endl;
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					stream << data[i * width + j];
				}
			}
		}

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
			generateTextures(font);

			this->fontList.push_back(font);
			return font;
		}
		void FontManager::generateTextures(FONT* font)
		{
			const size_t charRange = 256;
			const size_t charResolution = 512;
			struct BITMAP {
				unsigned char* data;
				size_t measurements;
				FT_Glyph_Metrics metrics;
				BITMAP(FT_GlyphSlot glyph) : metrics(glyph->metrics), measurements(glyph->bitmap.rows > glyph->bitmap.width ? glyph->bitmap.rows : glyph->bitmap.width)
				{
					this->data = new unsigned char[measurements * measurements];
				}
				~BITMAP()
				{
					delete[] data;
				}
				void copyData(unsigned char* newData, size_t height, size_t width)
				{
					if (height > this->measurements || width > this->measurements)
						throw std::out_of_range("given arguments are out of range");
					for (int i = 0; i < this->measurements; i++)
					{
						for (int j = 0; j < this->measurements; j++)
						{
							this->data[i * measurements + j] = (i >= height || j >= width ? 0 : newData[i * width + j]);
						}
					}
				}
				void clampTo(size_t newMeasurement)
				{
					auto newData = new unsigned char[newMeasurement * newMeasurement];
					for (size_t i = 0; i < newMeasurement; i++)
					{
						for (size_t j = 0; j < newMeasurement; j++)
						{
							newData[i * newMeasurement + j] = (i >= this->measurements || j >= this->measurements ? 0 : this->data[i * this->measurements + j]);
						}
					}
					this->measurements = newMeasurement;
					delete[] this->data;
					this->data = newData;
				}
			};
			std::vector<BITMAP*> bitmaps;
			bool failFlag = false;
			int fontMaxAdvance = 0;
			int fontMinBearingY = 0;
			int fontMaxBearingY = 0;
			FT_Error err;
			err = FT_Set_Pixel_Sizes(font->face, charResolution, charResolution);
			//err = FT_Set_Char_Size(font->face, charResolution * 64, 0, charResolution, 0);
			if (err != FT_Err_Ok)
			{
				LOGGER_WRITE(Logger::ERRORmsg, std::string("Cannot set CharResolution '").append(std::to_string(charResolution)).append("'"));
				return;
			}
			//Iterate through E-ASCII
			font->material->textures.resize(charRange);
			font->material->gpuTextures.resize(charRange);
			for (unsigned short c = 0; c < charRange; c++)
			{
				err = FT_Load_Char(font->face, c, FT_LOAD_RENDER);
				if (err != FT_Err_Ok)
				{
					char stringWorkaround[2] = { c, 0x00 };
					LOGGER_WRITE(Logger::WARNING, std::string("Failed to generate character '").append(stringWorkaround).append("'"));
					failFlag = true;
					continue;
				}

				if (font->face->glyph->metrics.horiAdvance > fontMaxAdvance)
					fontMaxAdvance = font->face->glyph->metrics.horiAdvance;
				if (font->face->glyph->metrics.horiBearingY > fontMaxBearingY)
					fontMaxBearingY = font->face->glyph->metrics.horiBearingY;
				if (font->face->glyph->metrics.horiBearingY - font->face->glyph->metrics.height < fontMinBearingY)
					fontMinBearingY = font->face->glyph->metrics.horiBearingY - font->face->glyph->metrics.height;
				//Extract bitmap
				BITMAP* bmp = new BITMAP(font->face->glyph);
				bmp->copyData(font->face->glyph->bitmap.buffer, font->face->glyph->bitmap.rows, font->face->glyph->bitmap.width);
				bitmaps.push_back(bmp);
				//char sTmp[] = { (unsigned char)c, 0x00 };
				//if (!std::iscntrl(c))
				//	exportToPGM(font->face->glyph->bitmap.buffer, font->face->glyph->bitmap.width, font->face->glyph->bitmap.rows, std::string("debugOut\\").append(sTmp).append("_FreeType_").append(std::to_string(c)).append(".pgm").c_str());

				//char sTmp[] = { (unsigned char)c, 0x00 };
				//if (!std::iscntrl(c))
				//	exportToPGM(bmp->data, bmp->measurements, bmp->measurements, std::string("debugOut\\").append(sTmp).append("_ClampedFreeType_").append(std::to_string(c)).append(".pgm").c_str());

			}
			BITMAP* yBitmap = bitmaps['y'];
			fontMaxBearingY += (yBitmap->metrics.horiBearingY - yBitmap->metrics.height) / 2 - 8 * 64;
			for (size_t i = 0; i < charRange; i++)
			{
				auto& it = bitmaps[i];
				auto texture = new TEXTURE();

				font->material->textures[i] = texture;
				font->material->gpuTextures[i] = 0x00;

				int xPixelOffset = (it->metrics.horiBearingX) / (double)fontMaxAdvance * charResolution;
				int yPixelOffset = (fontMaxBearingY - it->metrics.horiBearingY) / (double)fontMaxAdvance * charResolution;//(fontMaxHeight + fontMinHang + it->metrics.horiBearingY - it->metrics.height) / (double)fontMaxAdvance * charResolution;
				int wPixel = it->metrics.width / 64;
				int hPixel = it->metrics.height / 64;

				texture->trueHeight = it->metrics.height / 64;
				texture->trueWidth = it->metrics.width / 64;
				texture->subname = (char)i;
				texture->sizeOfDataArray = charResolution * charResolution;
				texture->data = new unsigned char[texture->sizeOfDataArray];
				texture->height = charResolution;
				texture->width = charResolution;
				texture->internalFormat = 1;
				texture->format = GL_RED;

				for (int i = 0; i < charResolution; i++)
				{
					for (int j = 0; j < charResolution; j++)
					{
						if (i - yPixelOffset >= 0		&& j - xPixelOffset >= 0 &&
							i - yPixelOffset < hPixel	&& j - xPixelOffset < wPixel)
						{
							texture->data[i * charResolution + j] = it->data[(i - yPixelOffset) * it->measurements + (j - xPixelOffset)];
						}
						else
						{
							texture->data[i * charResolution + j] = 0;
						}
					}
				}
				//char sTmp[] = { (unsigned char)i, 0x00 };
				//if (!std::iscntrl(i))
				//	exportToPGM(texture->data, charResolution, charResolution, std::string("debugOut\\").append(sTmp).append("_ShiftedFreeType_").append(std::to_string(i)).append(".pgm").c_str());
			}

			//Cleanup
			for (auto& it : bitmaps)
				delete it;
			bitmaps.clear();
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
				FT_Error err;
				//err = FT_Set_Char_Size(font->face, 256, 0, 100, 0);
				err = FT_Set_Pixel_Sizes(font->face, 512, 512);
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
				texture->textureUnit = 0;
				font->material->textures[c] = texture;

				texture->trueHeight = font->face->glyph->bitmap.rows;
				texture->trueWidth = font->face->glyph->bitmap.width;
				texture->subname = c;
				unsigned int largerSide = (texture->trueHeight > texture->trueWidth ? texture->trueHeight : texture->trueWidth);
				unsigned int powerOf2 = 1;
				while(largerSide > powerOf2)
					powerOf2 <<= 1;
				if (largerSide > powerOf2)
					LOGGER_WRITE(::Logger::ERRORmsg, "LargerSide > PowerOf2 During text glyph creation");
				texture->data = new unsigned char[powerOf2 * powerOf2 * 4];
				
				for (int i = 0; i < powerOf2; i++)
					for (int j = 0; j < powerOf2; j++)
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