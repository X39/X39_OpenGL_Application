#pragma once

#include "globals.h"

#include <Windows.h>
#include <WindowsX.h>
namespace X39
{
	namespace Singletons
	{
#define MOUSEMODE_GAMECAMERA 0
#define MOUSEMODE_MENU 1
		class Mouse
		{
			private:
				unsigned int mode;
				bool centered;

				Mouse(void);
			public:
				~Mouse(void);
				static Mouse& getInstance(void);
				void setMode(unsigned int m);
				unsigned int getMode(void);
				void showPointer(bool flag);
				void keepCenter(bool flag);
				void handleMouseMove(int posX, int posY);
				void handleMouseButtonEvent(ULONG ulButtons, USHORT usButtonData);
		};
	}
}
