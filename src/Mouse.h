#pragma once

#include <Windows.h>
#include <vector>

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
				std::vector<bool ( *)(int, int)> mouseMoveEventHandles;
				std::vector<bool ( *)(LPPOINT, ULONG, USHORT)> mouseClickEventHandles;

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

				std::vector<bool ( *)(int, int)>::iterator registerEventHandler(bool (*fnc)(int, int));
				void removeEventHandler(std::vector<bool ( *)(int, int)>::iterator eventHandle);
				
				std::vector<bool ( *)(LPPOINT, ULONG, USHORT)>::iterator registerEventHandler(bool (*fnc)(LPPOINT, ULONG, USHORT));
				void removeEventHandler(std::vector<bool ( *)(LPPOINT, ULONG, USHORT)>::iterator eventHandle);
		};
	}
}
