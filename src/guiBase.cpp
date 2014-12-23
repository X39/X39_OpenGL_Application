#include "globals.h"

#include "GuiBase.h"
namespace X39
{
	namespace GUI
	{
		GuiBase::GuiBase(void)
		{
		}

		GuiBase::~GuiBase(void)
		{
		}


		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
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

		void GuiBase::drawText2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY)
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
					drawTextLine2D(mat, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset);
					break;
				}
				pnt[0] = 0x00;
				drawTextLine2D(mat, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset);
				lastStr = pnt + 1;
				heightOffset++;
			}
		}

		void GuiBase::drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY)
		{
			int strLength = strlen(s);
			drawTextLine2D(mat, strLength, s, uiPosX, uiPosY, FONTSIZEBASE * size * strLength, (mat->textures[0].height / 32.0) * size);
		}

		void GuiBase::drawTextLine2D(::X39::Singletons::MATERIAL* mat, int strLength, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
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

		void GuiBase::drawChar2D(::X39::Singletons::MATERIAL* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
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
		
		void GuiBase::draw(void)
		{
			for(int i = 0; i < this->children.size(); i++)
				this->children[i].draw();
			v_draw();
		}
		bool GuiBase::mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i].mouseClick(mousePosition, ulButtons, usButtonData))
					break;
			return this->v_mouseClick(mousePosition, ulButtons, usButtonData);
		}

		bool GuiBase::mouseMove(int posX, int posY)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i].mouseMove(posX, posY))
					break;
			return this->v_mouseMove(posX, posY);
		}

		bool GuiBase::keyPressed(int mode, USHORT key)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i].keyPressed(mode, key))
					break;
			return this->v_keyPressed(mode, key);
		}

		void GuiBase::v_draw(){}
		bool GuiBase::v_mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData){return false;}
		bool GuiBase::v_mouseMove(int posX, int posY){return false;}
		bool GuiBase::v_keyPressed(int mode, USHORT key){return false;}
	}
}