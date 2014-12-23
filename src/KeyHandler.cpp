#include "KeyDefines.h"
#include "KeyEventCodes.h"
#include "GameCamera.h"

#include <map>
#include <Windows.h>
#include <vector>

#include "KeyHandler.h"

namespace X39
{
	namespace Singletons
	{

		KeyHandler::KeyHandler(void)
		{
			handleRawKey_SkipNext = 0;
			keyEventHandles = std::vector<bool (*)(int, USHORT)>();
			pPressedKeys = new std::map<int, bool>();
		}


		KeyHandler::~KeyHandler(void)
		{
			delete pPressedKeys;
		}

		void KeyHandler::KeyEvent(USHORT key, bool keyDown)
		{
			//trigger keyEvent on change
			if((*pPressedKeys)[key] != keyDown)
				for(std::vector<bool (*)(int, USHORT)>::iterator index = keyEventHandles.begin(); index != keyEventHandles.end(); index++)
				{
					if(*index != NULL)
					{
						if((*index)((keyDown ? KEYEVENT_KEYDOWN : KEYEVENT_KEYUP), key))
							break;
					}
				}
#if _DEBUG
			printf("[Key]\tCatched key: %i %s\n", key, keyDown ? "down" : "up");
#endif
			(*pPressedKeys)[key] = keyDown;
		}
		
		void KeyHandler::handleRawKey(USHORT rawKeyCode, USHORT flags)
		{
			if(handleRawKey_SkipNext > 0)
			{
				handleRawKey_SkipNext--;
				return;
			}
			if(flags < 2)
			{//nothing else to do for processing
				this->KeyEvent(rawKeyCode, flags == 0 ? true : false);
			}else
			{
				switch(rawKeyCode)
				{
					case EngineKeySet::KEY_LCTRL:
					{
						if(flags < 4)
							this->KeyEvent(EngineKeySet::KEY_RCTRL, flags == 2 ? true : false);
						else if(flags < 6)
						{

							handleRawKey_SkipNext = 3;
						}
					}
					break;
				}
			}
		}
		bool KeyHandler::isKeyPressed(EngineKeySet::KeySet key)
		{
			return (*pPressedKeys)[key]; 
		}
		std::vector<bool (*)(int, USHORT)>::iterator KeyHandler::registerEventHandler(bool (*fnc)(int, USHORT))
		{
			for(std::vector<bool (*)(int, USHORT)>::iterator index = keyEventHandles.begin(); index != keyEventHandles.end(); index++)
			{
				if(*index == NULL)
				{
					*index = fnc;
					return index;
				}
			}
			keyEventHandles.push_back(fnc);
			return keyEventHandles.end() - 1;
		}
		void KeyHandler::removeEventHandler(std::vector<bool (*)(int, USHORT)>::iterator eventHandle)
		{
			*eventHandle = NULL;
		}
		KeyHandler& KeyHandler::getInstance(void)
		{
			static KeyHandler instance;
			return instance;
		}
	}
}
