#include "Mouse.h"
#define SENSITIVY 0.5;

namespace X39
{
	namespace Singletons
	{
		Mouse::Mouse(void)
		{
			setMode(MOUSEMODE_MENU);
			centered = true;
		}


		Mouse::~Mouse(void)
		{
		}

		Mouse& Mouse::getInstance(void)
		{
			static Mouse instance;
			return instance;
		}
		void Mouse::setMode(unsigned int m)
		{
			mode = m;
			switch(mode)
			{
				case MOUSEMODE_GAMECAMERA:
					showPointer(false);
					keepCenter(true);
					break;
				case MOUSEMODE_MENU:
					showPointer(true);
					keepCenter(false);
					break;
			}
		}
		unsigned int Mouse::getMode(void)
		{
			return mode;
		}
		void Mouse::showPointer(bool flag)
		{
			if(flag)
				ShowCursor(flag);
			else
				while(ShowCursor(flag) >= 0);
		}
		void Mouse::keepCenter(bool flag)
		{
			this->centered = flag;
		}
		void Mouse::handleMouseMove(int posX, int posY)
		{
			if(mode == MOUSEMODE_GAMECAMERA)
			{
				//printf("%i", invertPitch);
				double yawModificator = posX * SENSITIVY;
				double pitchModificator = posY * SENSITIVY;
				if(yawModificator > 10)
				{
					yawModificator = 10;
				}
				if(pitchModificator > 10)
				{
					pitchModificator = 10;
				}
				::X39::Singletons::GameCamera::getInstance().setYaw(yawModificator + ::X39::Singletons::GameCamera::getInstance().getYaw());
				::X39::Singletons::GameCamera::getInstance().setPitch(pitchModificator + ::X39::Singletons::GameCamera::getInstance().getPitch());
				if(centered)
				{
					POINT pt = {::GLOBAL::RENDER::height / 2, ::GLOBAL::RENDER::width / 2};
					ClientToScreen(::GLOBAL::windowHandle, &pt);
					SetCursorPos(pt.x, pt.y);
				}
				//skipNext = true;
			}
		}
		void Mouse::handleMouseButtonEvent(ULONG ulButtons, USHORT usButtonData)
		{
			if(mode == MOUSEMODE_MENU)
			{
				POINT p;
				if(!GetCursorPos(&p))
				{
					LOGGER_CODE(char str[256];)
					LOGGER_CODE(sprintf(str, "error while receiving mouse position: %d", GetLastError());)
					LOGGER_WRITE(::Logger::ERRORmsg, str);
				}
				else
				{
					LOGGER_WRITE(::Logger::USERINPUT, p);
				}
			}
		}
	}
}
