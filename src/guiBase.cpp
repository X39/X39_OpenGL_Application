#include "globals.h"
#include "GuiBase.h"
#include "Shader.h"
namespace X39
{
	namespace GUI
	{
		GuiBase::GuiBase(bool focusable)
		{
			enabled = true;
			isFocusable = focusable;
			focusedGUI = NULL;
		}

		GuiBase::~GuiBase(void)
		{
		}
		void GuiBase::addChild(GuiBase* child)
		{
			children.push_back(child);
		}

		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			drawTexture2D(mat, 0, tPosX, tPosY, tWidth, tHeight, uiPosX, uiPosY, uiWidth, uiHeight);
		}
		//static GLuint vbo_textDrawingVertices = 0;
		//static GLuint vbo_textDrawingIndexes = 0;
		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			//if(vbo_textDrawingVertices == 0)
			//{
			//	glGenBuffersARB(1, &vbo_textDrawingVertices);
			//	glGenBuffersARB(1, &vbo_textDrawingIndexes);
			//	GLfloat vertices[] = {0, 1, 2, 3};
			//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_textDrawingIndexes);
			//	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STATIC_DRAW_ARB);
			//}
			::X39::Singletons::TEXTURE* texture = mat->textures[textureIndex];
			double textureWidth = texture->width;
			double textureHeight = texture->height;
			//if(texture->trueHeight != 0)
			//	uiPosY += ((texture->height - texture->trueHeight) / (double)texture->height) * uiHeight / 2;
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(mat, textureIndex);
			//glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_textDrawingVertices);
			//glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_textDrawingIndexes);
//			::X39::Singletons::FontManager::getInstance().fontShader.use(0);
			//GLfloat vertices[] = {
			//						uiPosX,				uiPosY,
			//						uiPosX + uiWidth,	uiPosY,
			//						uiPosX + uiWidth,	uiPosY +uiHeight,
			//						uiPosX,				uiPosY +uiHeight
			//					 };
			//glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), vertices, GL_STREAM_DRAW_ARB);
			//glEnableClientState(GL_VERTEX_ARRAY);
			//glVertexPointer(3, GL_FLOAT, 0, 0);
			//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);
			//glDisableClientState(GL_VERTEX_ARRAY);
			//glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
			//glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
			glBegin(GL_QUADS);
				glTexCoord2f(tPosX / textureWidth,				tPosY / textureHeight				);	glVertex2f(uiPosX,				uiPosY			);
				glTexCoord2f((tPosX + tWidth) / textureWidth,	tPosY / textureHeight				);	glVertex2f(uiPosX + uiWidth,	uiPosY			);
				glTexCoord2f((tPosX + tWidth) / textureWidth,	(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX + uiWidth,	uiPosY +uiHeight);
				glTexCoord2f(tPosX / textureWidth,				(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX,				uiPosY +uiHeight);
			glEnd();

//			::X39::Singletons::FontManager::getInstance().fontShader.unuse();
		}
		
		void GuiBase::drawText2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY)
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
					drawTextLine2D(font, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset);
					break;
				}
				pnt[0] = 0x00;
				drawTextLine2D(font, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset);
				lastStr = pnt + 1;
				heightOffset++;
			}
		}
		
		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY)
		{
			int strLength = strlen(s);
			drawTextLine2D(font, strLength, s, uiPosX, uiPosY, FONTSIZEBASE * size * strLength, 12 * size);
		}
		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY, double maxWidth)
		{
			int strLength = strlen(s);
			double width = FONTSIZEBASE * size * strLength;
			if(width > maxWidth)
			{
				strLength -= (int)((width - maxWidth) / FONTSIZEBASE / size + 0.5);
				width = FONTSIZEBASE * size * strLength;
			}
			drawTextLine2D(font, strLength, s, uiPosX, uiPosY, width, FONTSIZEBASE * size);
		}

		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, int strLength, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			char c;
			for(int i = 0; i < strLength; i++)
			{
				c = s[i];
				if(c == 0x00)
					break;
				if(c == ' ')
					continue;
				drawChar2D(font, c, uiPosX + (uiWidth / strLength * i), uiPosY, uiWidth / strLength, uiHeight);
			};
		}

		void GuiBase::drawChar2D(::X39::Singletons::FONT* font, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			int charCode = c;
			if(charCode < 0)
				charCode = 0;
			if(charCode > 255)
				return;
			unsigned int fontIndex = ::X39::Singletons::FontManager::getInstance().getCharTextureIndex(font, c);
			float w = font->material->textures[fontIndex]->width;
			float h = font->material->textures[fontIndex]->height;
			drawTexture2D(font->material, fontIndex, charCode % 32 * w, charCode / 32 * h, w, h, uiPosX, uiPosY, uiWidth, uiHeight);
		}
		
		bool GuiBase::isFocused(void)
		{
			return ::X39::GlobalObject::getInstance().mainDisplay->focusedGUI == this;
		}
		void GuiBase::setFocused(void)
		{
			setFocused(this);
		}
		void GuiBase::setFocused(GuiBase* ui)
		{
			if(!ui->isFocusable)
				return;
			::X39::GlobalObject::getInstance().mainDisplay->focusedGUI = ui;
		}
		bool GuiBase::isEnabled(void)
		{
			return enabled;
		}
		void GuiBase::setEnabled(bool flag)
		{
			enabled = flag;
		}
		
		void GuiBase::draw(void)
		{
			for(int i = 0; i < this->children.size(); i++)
				this->children[i]->draw();
			e_draw();
		}

		bool GuiBase::mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->mouseClick(mousePos, ulButtons, usButtonData))
					break;
			if(!this->isEnabled())
				return false;
			return this->e_mouseClick(mousePos, ulButtons, usButtonData);
		}

		bool GuiBase::mouseMove(int posX, int posY, LPPOINT mousePos)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->mouseMove(posX, posY, mousePos))
					break;
			if(!this->isEnabled())
				return false;
			return this->e_mouseMove(posX, posY, mousePos);
		}

		bool GuiBase::keyPressed(int mode, USHORT key)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->keyPressed(mode, key))
					break;
			if(!this->isEnabled())
				return false;
			return this->e_keyPressed(mode, key);
		}

		void GuiBase::e_draw(){}
		bool GuiBase::e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData){return false;}
		bool GuiBase::e_mouseMove(int posX, int posY, LPPOINT mousePos){return false;}
		bool GuiBase::e_keyPressed(int mode, USHORT key){return false;}
	}
}