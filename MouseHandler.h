#pragma once
#include "globals.h"
#include "GameCamera.h"
#include <Windows.h>
#include <WindowsX.h>
namespace X39
{
	namespace EngineObjects
	{
		class MouseHandler
		{
		private:
				double sensitivy;
				bool hasFocus;
				bool skipNext;
				bool invertPitch;

		public:
			MouseHandler(void);
			~MouseHandler(void);

			void mouseMove(int posX, int posY);
			void mouseButtonEvent(ULONG ulButtons, USHORT usButtonData);
			void setFocus(bool flag);
			void showCursor(bool flag);
		};
	};
};
