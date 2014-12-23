#pragma once

#include <vector>

#define FONTSIZEBASE 8
#define LINESPACING 5

namespace X39
{
	namespace GUI
	{
		class GuiBase
		{
			private:
				std::vector<GuiBase> children;

			public:
				GuiBase(void);
				~GuiBase(void);
		
				static void drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawText2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, int strLen, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawChar2D(::X39::Singletons::MATERIAL* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				
				void draw(void);
				bool mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData);
				bool mouseMove(int posX, int posY);
				bool keyPressed(int mode, USHORT key);

				virtual void v_draw(void);
				virtual bool v_mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData);
				virtual bool v_mouseMove(int posX, int posY);
				virtual bool v_keyPressed(int mode, USHORT key);
		};
	};
};