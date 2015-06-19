#include "globals.h"
#include "GuiBase.h"
#include "Shader.h"
#include <array>
#include <glm\gtx\euler_angles.hpp>
namespace X39
{
	namespace GUI
	{

		GLuint structuredVAO = 0;
		static inline void genStructuredVAO(void)
		{
			if (structuredVAO == 0)
			{
				glGenVertexArrays(1, &structuredVAO);
				glBindVertexArray(structuredVAO);

				GLuint vertBufferID, indexBufferID;
				glGenBuffers(1, &vertBufferID);
				glGenBuffers(1, &indexBufferID);

				struct Vec2 { float x, y; };
				struct Vec3 { float x, y, z; };
				struct Vert { Vec3 pos; Vec2 tex; };

				std::array<Vert, 4>  verts = { {
					{ { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
					{ { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
					{ { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
					{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } }
					} };

				std::array<unsigned int, 6> indexies = { { 0, 1, 2, 3 } };

				// Vertex buffer
				glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * verts.size(), verts.data(), GL_STATIC_DRAW);
				glEnableVertexAttribArray(0); // Matches layout (location = 0)
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
				glEnableVertexAttribArray(1); // Matches layout (location = 1)
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

				// Index buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexies.size(), indexies.data(), GL_STATIC_DRAW);
				glBindVertexArray(0);
			}
		}
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

		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, Shader& shad)
		{
			drawTexture2D(mat, 0, tPosX, tPosY, tWidth, tHeight, uiPosX, uiPosY, uiWidth, uiHeight, shad);
		}
		//static GLuint vbo_textDrawingIndexes = 0;
		void GuiBase::drawTexture2D(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, Shader& shad)
		{
			genStructuredVAO();
			::X39::Singletons::TEXTURE* texture = mat->textures[textureIndex];
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(mat, textureIndex);

			::glm::mat4 scaleMatrix = ::glm::scale(::glm::mat4(1.0f), ::glm::vec3((float)uiWidth, (float)uiHeight, 1.0f));
			std::array<float, 6> uvManipulation = { tPosX, tPosY, tWidth, tHeight, texture->width, texture->height };
			glm::mat4 projectionMatrix = glm::ortho(
				(float)0,
				(float)::X39::GlobalObject::getInstance().render_width,
				(float)::X39::GlobalObject::getInstance().render_height,
				(float)0,
				0.0f,
				100.0f
				);

			glBindVertexArray(structuredVAO);

			shad.setUniformMatrix4fv("scaleMatrix", 1, GL_FALSE, &scaleMatrix[0][0], -1);
			shad.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
			shad.setUniform1fv("UV_Manipulators", 6, uvManipulation.data(), -1);
			shad.setUniform3fv("worldPosition", 1, &glm::vec3(
				uiPosX / uiWidth,
				uiPosY / uiHeight,
				0
				)[0], -1);
			shad.setUniform1i("textureSampler", texture->textureUnit, -1);
			glDrawElements(GL_QUADS, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


		}
		void GuiBase::drawTexture2DColored(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red, float green, float blue, float alpha, Shader& shad)
		{
			genStructuredVAO();
			::X39::Singletons::TEXTURE* texture = mat->textures[textureIndex];
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(mat, textureIndex);

			::glm::mat4 scaleMatrix = ::glm::scale(::glm::mat4(1.0f), ::glm::vec3((float)uiWidth, (float)uiHeight, 1.0f));
			std::array<float, 6> uvManipulation = { tPosX, tPosY, tWidth, tHeight, texture->width, texture->height };
			std::array<float, 4> color_manipulator = { red, green, blue, alpha };
			glm::mat4 projectionMatrix = glm::ortho(
				(float)0,
				(float)::X39::GlobalObject::getInstance().render_width,
				(float)::X39::GlobalObject::getInstance().render_height,
				(float)0,
				0.0f,
				100.0f
				);

			glBindVertexArray(structuredVAO);

			shad.setUniformMatrix4fv("scaleMatrix", 1, GL_FALSE, &scaleMatrix[0][0], -1);
			shad.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
			shad.setUniform1fv("UV_Manipulators", 6, uvManipulation.data(), -1);
			shad.setUniform1fv("color_manipulator", 4, color_manipulator.data(), -1);
			shad.setUniform3fv("worldPosition", 1, &glm::vec3(
				uiPosX / uiWidth,
				uiPosY / uiHeight,
				0
				)[0], -1);
			shad.setUniform1i("textureSampler", texture->textureUnit, -1);
			glDrawElements(GL_QUADS, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);


		}

		void GuiBase::drawText2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY, float red, float green, float blue, float alpha)
		{
			int heightOffset = 0;
			int strLength = strlen(s);
			char* str = new char[strLength];
			char* lastStr = str;
			strcpy(str, s);
			while (true)
			{
				char* pnt = strchr(lastStr, '\n');
				if (pnt == NULL)
				{
					drawTextLine2D(font, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset, red, green, blue, alpha);
					break;
				}
				pnt[0] = 0x00;
				drawTextLine2D(font, lastStr, size, uiPosX, uiPosY + (FONTSIZEBASE * size * heightOffset) + LINESPACING * heightOffset, red, green, blue, alpha);
				lastStr = pnt + 1;
				heightOffset++;
			}
		}

		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY, float red, float green, float blue, float alpha)
		{
			int strLength = strlen(s);
			drawTextLine2D(font, strLength, s, uiPosX, uiPosY, FONTSIZEBASE * size * strLength, 12 * size, red, green, blue, alpha);
		}
		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, const char* s, float size, double uiPosX, double uiPosY, double maxWidth, float red, float green, float blue, float alpha)
		{
			int strLength = strlen(s);
			double width = FONTSIZEBASE * size * strLength;
			if (width > maxWidth)
			{
				strLength -= (int)((width - maxWidth) / FONTSIZEBASE / size + 0.5);
				width = FONTSIZEBASE * size * strLength;
			}
			drawTextLine2D(font, strLength, s, uiPosX, uiPosY, width, FONTSIZEBASE * size, red, green, blue, alpha);
		}

		void GuiBase::drawTextLine2D(::X39::Singletons::FONT* font, int strLength, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red, float green, float blue, float alpha)
		{
			char c;
			for (int i = 0; i < strLength; i++)
			{
				c = s[i];
				if (c == 0x00)
					break;
				if (c == ' ')
					continue;
				drawChar2D(font, c, uiPosX + (uiWidth / strLength * i), uiPosY, uiWidth / strLength, uiHeight, red, green, blue, alpha);
			};
		}

		void GuiBase::drawChar2D(::X39::Singletons::FONT* font, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red, float green, float blue, float alpha)
		{
			Shader& shad = ::X39::Singletons::FontManager::getInstance().fontShader;
			shad.use();
			int charCode = c;
			if (charCode < 0)
				charCode = 0;
			if (charCode > 255)
				return;
			unsigned int fontIndex = ::X39::Singletons::FontManager::getInstance().getCharTextureIndex(font, c);
			double w = (double)font->material->textures[fontIndex]->width;
			double h = (double)font->material->textures[fontIndex]->height;
			//drawTexture2D(font->material, fontIndex, charCode % 32 * w, charCode / 32 * h, w, h, uiPosX, uiPosY, uiWidth, uiHeight);
			drawTexture2DColored(font->material, fontIndex, 0, 0, w, h, uiPosX, uiPosY, uiWidth, uiHeight, red, green, blue, alpha, shad);
			shad.unuse();
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
			if (!ui->isFocusable)
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
			for (int i = 0; i < this->children.size(); i++)
				this->children[i]->draw();
			e_draw();
		}

		bool GuiBase::mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			for (int i = 0; i < this->children.size(); i++)
				if (this->children[i]->isEnabled() && this->children[i]->mouseClick(mousePos, ulButtons, usButtonData))
					break;
			if (!this->isEnabled())
				return false;
			return this->e_mouseClick(mousePos, ulButtons, usButtonData);
		}

		bool GuiBase::mouseMove(int posX, int posY, LPPOINT mousePos)
		{
			for (int i = 0; i < this->children.size(); i++)
				if (this->children[i]->isEnabled() && this->children[i]->mouseMove(posX, posY, mousePos))
					break;
			if (!this->isEnabled())
				return false;
			return this->e_mouseMove(posX, posY, mousePos);
		}

		bool GuiBase::keyPressed(int mode, USHORT key)
		{
			for (int i = 0; i < this->children.size(); i++)
				if (this->children[i]->isEnabled() && this->children[i]->keyPressed(mode, key))
					break;
			if (!this->isEnabled())
				return false;
			return this->e_keyPressed(mode, key);
		}

		void GuiBase::e_draw(){}
		bool GuiBase::e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData){ return false; }
		bool GuiBase::e_mouseMove(int posX, int posY, LPPOINT mousePos){ return false; }
		bool GuiBase::e_keyPressed(int mode, USHORT key){ return false; }
		Shader& GuiBase::getShader(void)
		{
			return ::X39::Singletons::FontManager::getInstance().fontShader;
		}
	}
}