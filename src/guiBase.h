#pragma once

#include "globals.h"
namespace X39
{
	namespace GUI
	{
		class guiBase
		{
			protected:

			public:
				guiBase(void);
				~guiBase(void);
		
				static void drawTexture2D(::X39::Singletons::MATERIAL* mat, double tPosX, double tPosY, double tWidth, double tHeight, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawText2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, const char* s, float size, double uiPosX, double uiPosY);
				static void drawTextLine2D(::X39::Singletons::MATERIAL* mat, int strLen, const char* s, double uiPosX, double uiPosY, double uiWidth, double uiHeight);
				static void drawChar2D(::X39::Singletons::MATERIAL* mat, const char c, double uiPosX, double uiPosY, double uiWidth, double uiHeight);

				virtual void init();
				virtual void draw();
				virtual bool mouseClick(LPPOINT mousePosition, ULONG ulButtons, USHORT usButtonData);
				virtual bool mouseMove(int posX, int posY);
				virtual bool keyPressed(int mode, USHORT key);
		};
	};
};