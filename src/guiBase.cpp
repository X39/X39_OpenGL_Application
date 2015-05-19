#include "globals.h"
#include "GuiBase.h"
#include "Shader.h"
#include <array>
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
		static GLuint texture2D_vao = 0;
		//static GLuint vbo_textDrawingIndexes = 0;
		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight)
		{
			if (texture2D_vao == 0)
			{
				glGenVertexArrays(1, &texture2D_vao);
				glBindVertexArray(texture2D_vao);

				GLuint vertBufferID, indexBufferID;
				glGenBuffers(1, &vertBufferID);
				glGenBuffers(1, &indexBufferID);

				struct Vec2 { float x, y; };
				struct Vec3 { float x, y, z; };
				struct Vert { Vec3 pos; Vec2 tex; };

				std::array<Vert, 4> cubeVerts = { {
					{ { -0.5f, -0.5f }, { -0.5f, -0.5f } },	//bottom-left
					{ { -0.5f, 0.5f }, { -0.5f, 0.5f } },	//bottom-right
					{ { 0.5f, -0.5f }, { 0.5f, -0.5f } },	//top-left
					{ { 0.5f, 0.5f }, { 0.5f, 0.5f } }		//top-right
					} };

				std::array<unsigned int, 4> cubeIdxs = { { 0, 1, 2, 3 } };

				// Vertex buffer
				glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
				glEnableVertexAttribArray(0); // Matches layout (location = 0)
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
				glEnableVertexAttribArray(1); // Matches layout (location = 1)
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

				// Index buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
				glBindVertexArray(0);
			}
			::X39::Singletons::TEXTURE* texture = mat->textures[textureIndex];
			double textureWidth = texture->width;
			double textureHeight = texture->height;
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(mat, textureIndex);
			Shader& shad = ::X39::Singletons::FontManager::getInstance().fontShader;
			shad.use();
			::glm::mat4 scaleMatrix = ::glm::scale(::glm::mat4(1.0f), ::glm::vec3((float)uiWidth, (float)uiHeight, 10.f));
			glBindVertexArray(texture2D_vao);
			//shad.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, &viewMatrix[0][0], -1);
			//shad.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
			//shad.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, &glm::mat4()[0][0], 0);
			shad.setUniform2f("textureMeasurements", (float)textureWidth, (float)textureHeight, -1);
			shad.setUniformMatrix4fv("scaleMatrix", 1, GL_FALSE, &scaleMatrix[0][0], -1);
			shad.setUniform2f("screenPosition", (float)uiPosX, (float)uiPosY, -1);
			shad.setUniform1i("texture_sampler", GL_TEXTURE0, -1);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			shad.unuse();


			//glBegin(GL_QUADS);
			//	glTexCoord2f(tPosX / textureWidth,				tPosY / textureHeight				);	glVertex2f(uiPosX,				uiPosY				);
			//	glTexCoord2f((tPosX + tWidth) / textureWidth,	tPosY / textureHeight				);	glVertex2f(uiPosX + uiWidth,	uiPosY				);
			//	glTexCoord2f((tPosX + tWidth) / textureWidth,	(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX + uiWidth,	uiPosY + uiHeight	);
			//	glTexCoord2f(tPosX / textureWidth,				(tPosY + tHeight) / textureHeight	);	glVertex2f(uiPosX,				uiPosY + uiHeight	);
			//glEnd();

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
			double w = (double)font->material->textures[fontIndex]->width;
			double h = (double)font->material->textures[fontIndex]->height;
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