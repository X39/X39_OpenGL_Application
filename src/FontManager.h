#pragma once
#include "MaterialManager.h"
#include "Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>

namespace X39
{
	namespace Singletons
	{
		
		typedef struct stFont{
			FT_Face face;
			MATERIAL* material;
		}FONT;
		class FontManager
		{
		private:
			FT_Library library;
			std::vector<FONT*> fontList;

		public:
			::X39::Shader fontShader;
			FontManager(void);
			~FontManager(void);

			FONT* registerFont(std::string path);

			FONT* getFont(unsigned int index);
			
			unsigned int getCharTextureIndex(FONT* font, char c);

			static FontManager& getInstance();
		private:
			void generateTextures(FONT* font);
		};
	};
};