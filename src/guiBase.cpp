#include "guiBase.h"


namespace X39
{
	namespace GUI
	{
		guiBase::guiBase(void)
		{
		}
		guiBase::~guiBase(void)
		{
		}


		void guiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			double textureWidth = mat->textures[TEXTURE_BASETEXTURE].width;
			double textureHeight = mat->textures[TEXTURE_BASETEXTURE].height;
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(mat);
			glBegin(GL_QUADS);
				glTexCoord2f(tPosX / textureWidth,				tPosY / textureHeight				);	glVertex2f(uiPosX,				uiPosY			);
				glTexCoord2f((tPosX + tWidth) / textureWidth,	tPosY / textureHeight				);	glVertex2f(uiPosX + uiWidth,	uiPosY			);
				glTexCoord2f((tPosX + tWidth) / textureWidth,	(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX + uiWidth,	uiPosY +uiHeight);
				glTexCoord2f(tPosX / textureWidth,				(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX,				uiPosY +uiHeight);
			glEnd();
		}
		void guiBase::drawText2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY)
		{
			int heightOffset = 0;
			int strLength = strlen(s);
			char* str = new char[strLength];
			char* lastStr = str;
			strcpy(str, s);
			while(true)
			{
				char* pnt = strchr(lastStr, '\n');
				if(pnt == NULL)
				{
					drawTextLine2D(mat, lastStr, size, uiPosX, uiPosY + ((mat->textures[0].height / 32.0) * size * heightOffset));
					break;
				}
				pnt[0] = 0x00;
				drawTextLine2D(mat, lastStr, size, uiPosX, uiPosY + ((mat->textures[0].height / 32.0) * size * heightOffset));
				lastStr = pnt + 1;
				heightOffset++;
			}
		}
		void guiBase::drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY)
		{
			int strLength = strlen(s);
			drawTextLine2D(mat, strLength, s, uiPosX, uiPosY, (mat->textures[0].width / 32.0) * size * strLength, (mat->textures[0].height / 32.0) * size);
		}
		void guiBase::drawTextLine2D(::X39::Singletons::MATERIAL* mat, int strLength, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			char c;
			unsigned int index = 0;
			while(true)
			{
				c = s[index];
				if(c == 0x00)
					break;
				drawChar2D(mat, c, uiPosX + (uiWidth / strLength * index), uiPosY, uiWidth / strLength, uiHeight);
				index++;
			};
		}
		void guiBase::drawChar2D(::X39::Singletons::MATERIAL* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			int charCode = c;
			if(charCode < 0)
				charCode = 0;
			if(charCode > 255)
				return;
			float w = (mat->textures[0].width / 32.0);
			float h = (mat->textures[0].height / 32.0);
			drawTexture2D(mat, charCode % 32 * w, charCode / 32 * h, w, h, uiPosX, uiPosY, uiWidth, uiHeight);
		}

		void guiBase::init()
		{
		}
		void guiBase::draw()
		{
		}
		bool guiBase::mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData)
		{
			return false;
		}
		bool guiBase::mouseMove(int posX, int posY)
		{
			return false;
		}
		bool guiBase::keyPressed(int mode, USHORT key)
		{
			return false;
		}
	};
};