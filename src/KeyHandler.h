#pragma once

#include "KeyDefines.h"

#include <map>
#include <vector>

namespace X39
{
	namespace Singletons
	{
		class KeyHandler
		{
		private:
			std::map<int, bool>* pPressedKeys;
			std::vector<bool ( *)(int, USHORT)> keyEventHandles;
			USHORT handleRawKey_SkipNext;

			KeyHandler(void);
		public:
			~KeyHandler(void);
			void KeyEvent(USHORT key, bool keyDown);
			bool isKeyPressed(EngineKeySet::KeySet key);
			void handleRawKey(USHORT rawKeyCode, USHORT flags);

			std::vector<bool ( *)(int, USHORT)>::iterator registerEventHandler(bool (*fnc)(int, USHORT));
			void removeEventHandler(std::vector<bool ( *)(int, USHORT)>::iterator eventHandle);

			static KeyHandler& getInstance(void);
		};
	};
};
