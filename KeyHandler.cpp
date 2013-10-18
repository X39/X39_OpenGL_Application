#include "KeyHandler.h"

namespace X39
{
	namespace EngineObjects
	{

		KeyHandler::KeyHandler(void)
		{
			handleRawKey_SkipNext = 0;
			pPressedKeys = new std::map<int, bool>();
		}


		KeyHandler::~KeyHandler(void)
		{
			delete pPressedKeys;
		}

		void KeyHandler::KeyEvent(USHORT key, bool keyDown)
		{
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
	};
};
