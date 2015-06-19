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
			setMode(MouseMode::Menu);
			centered = false;
		}


		Mouse::~Mouse(void)
		{
		}

		Mouse& Mouse::getInstance(void)
		{
			static Mouse instance;
			return instance;
		}
		void Mouse::setMode(MouseMode m)
		{
			mode = m;
			switch (mode)
			{
				case MouseMode::Camera:
					showPointer(false);
					keepCenter(true);
					break;
				case MouseMode::Menu:
					showPointer(true);
					keepCenter(false);
					break;
			}
		}
		Mouse::MouseMode Mouse::getMode(void)
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
		void Mouse::keepCenter()
		{
			keepCenter(centered);
		}
		void Mouse::keepCenter(bool flag)
		{
			centered = flag;
			if(flag)
			{
				RECT rect;
				GetWindowRect(::X39::GlobalObject::getInstance().windowHandle, &rect);
				ClipCursor(&rect);
			}
			else
			{
				ClipCursor(NULL);
			}
		}
		void Mouse::handleMouseMove(int posX, int posY)
		{
			if (mode == MouseMode::Camera)
			{
				//printf("%i", invertPitch);
				double yawModificator = posX * SENSITIVY;
				double pitchModificator = -posY * SENSITIVY;
				if(yawModificator > 10)
				{
					yawModificator = 10;
				}
				if(pitchModificator > 10)
				{
					pitchModificator = 10;
				}
				::X39::Singletons::Camera::getInstance().setYaw(yawModificator + ::X39::Singletons::Camera::getInstance().getYaw());
				::X39::Singletons::Camera::getInstance().setPitch(pitchModificator + ::X39::Singletons::Camera::getInstance().getPitch());
			}
			
			POINT p;
			if (mode == MouseMode::Menu)
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
			if (mode == MouseMode::Menu)
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
