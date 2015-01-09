#pragma once

#include "MaterialManager.h"

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
				GuiBase(void);
				GuiBase(bool focusable);
				~GuiBase(void);

				void addChild(GuiBase* child);
		
				static void drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawText2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY, double maxWidth);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, int strLen, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawChar2D(::X39::Singletons::MATERIAL* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				
				bool isFocused(void);
				void setFocused(void);
				void setFocused(GuiBase* ui);

				bool isEnabled(void);
				void setEnabled(bool flag);

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