#include "globals.h"

#include <Windows.h>
#include <WindowsX.h>

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
					POINT pt = {::X39::GlobalObject::getInstance().render_height / 2, ::X39::GlobalObject::getInstance().render_width / 2};
					ClientToScreen(::X39::GlobalObject::getInstance().windowHandle, &pt);
					SetCursorPos(pt.x, pt.y);
				}
				//skipNext = true;
			}
			
			POINT p;
			if(mode == MOUSEMODE_MENU)
			{
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
			for(std::vector<bool (*)(int, int, LPPOINT)>::iterator index = mouseMoveEventHandles.begin(); index != mouseMoveEventHandles.end(); index++)
			{
				if(*index != NULL)
				{
					if((*index)(posX, posY, &p))
						break;
				}
			}
		}
		void Mouse::handleMouseButtonEvent(ULONG ulButtons, USHORT usButtonData)
		{
			POINT p;
			if(mode == MOUSEMODE_MENU)
			{
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
			for(std::vector<bool (*)(LPPOINT, ULONG, USHORT)>::iterator index = mouseClickEventHandles.begin(); index != mouseClickEventHandles.end(); index++)
			{
				if(*index != NULL)
				{
					if((*index)(&p, ulButtons, usButtonData))
						break;
				}
			}
		}
		
		std::vector<bool ( *)(int, int, LPPOINT)>::iterator Mouse::registerEventHandler(bool (*fnc)(int, int, LPPOINT))
		{
			for(std::vector<bool (*)(int, int, LPPOINT)>::iterator index = mouseMoveEventHandles.begin(); index != mouseMoveEventHandles.end(); index++)
			{
				if(*index == NULL)
				{
					*index = fnc;
					return index;
				}
			}
			mouseMoveEventHandles.push_back(fnc);
			return mouseMoveEventHandles.end() - 1;
		}
		void Mouse::removeEventHandler(std::vector<bool ( *)(int, int, LPPOINT)>::iterator eventHandle)
		{
			*eventHandle = NULL;
		}
		
		std::vector<bool ( *)(LPPOINT, ULONG, USHORT)>::iterator Mouse::registerEventHandler(bool (*fnc)(LPPOINT, ULONG, USHORT))
		{
			for(std::vector<bool (*)(LPPOINT, ULONG, USHORT)>::iterator index = mouseClickEventHandles.begin(); index != mouseClickEventHandles.end(); index++)
			{
				if(*index == NULL)
				{
					*index = fnc;
					return index;
				}
			}
			mouseClickEventHandles.push_back(fnc);
			return mouseClickEventHandles.end() - 1;
		}
		void Mouse::removeEventHandler(std::vector<bool ( *)(LPPOINT, ULONG, USHORT)>::iterator eventHandle)
		{
			*eventHandle = NULL;
		}
	}
}
