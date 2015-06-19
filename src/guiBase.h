#pragma once

#include "MaterialManager.h"
#include "FontManager.h"

#include <vector>
#include <Windows.h>

#define FONTSIZEBASE 12.0
#define LINESPACING 5

namespace X39
{
	namespace GUI
	{
		class GuiBase
		{
			private:
				GuiBase* focusedGUI;

			protected:
				std::vector<GuiBase*> children;
				bool enabled;
				bool isFocusable;


			public:
				GuiBase(bool focusable = true);
				~GuiBase(void);

				void addChild(GuiBase* child);
		
				static void drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, Shader& shad);
				static void drawTexture2D(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, Shader& shad);
				static void drawTexture2DColored(::X39::Singletons::MATERIAL* mat, unsigned int textureIndex, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red, float green, float blue, float alpha, Shader& shad);
				static void drawText2D(::X39::Singletons::FONT* mat, const char* s, float size, double uiPosX, double uiPosY, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
				static void drawTextLine2D(::X39::Singletons::FONT* mat, const char* s, float size, double uiPosX, double uiPosY, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
				static void drawTextLine2D(::X39::Singletons::FONT* mat, const char* s, float size, double uiPosX, double uiPosY, double maxWidth, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
				static void drawTextLine2D(::X39::Singletons::FONT* mat, int strLen, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
				static void drawChar2D(::X39::Singletons::FONT* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight, float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f);
				
				bool isFocused(void);
				void setFocused(void);
				void setFocused(GuiBase* ui);

				bool isEnabled(void);
				void setEnabled(bool flag);

				Shader& getShader(void);

				virtual void draw(void);
				virtual bool mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
				virtual bool mouseMove(int posX, int posY, LPPOINT mousePos);
				virtual bool keyPressed(int mode, USHORT key);

				virtual void e_draw(void);
				virtual bool e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
				virtual bool e_mouseMove(int posX, int posY, LPPOINT mousePos);
				virtual bool e_keyPressed(int mode, USHORT key);

		};
	};
};