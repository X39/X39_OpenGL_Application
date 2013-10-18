#pragma once

#include "KeyDefines.h"
#include "GameCamera.h"
#include "EntityFactory.h"
#include "EntityPlayer.h"

#include <map>
#include <Windows.h>

namespace X39
{
	namespace EngineObjects
	{
		class KeyHandler
		{
		private:
			std::map<int, bool>* pPressedKeys;
			USHORT handleRawKey_SkipNext;
		public:
			KeyHandler(void);
			~KeyHandler(void);
			void KeyEvent(USHORT key, bool keyDown);
			bool isKeyPressed(EngineKeySet::KeySet key);
			void handleRawKey(USHORT rawKeyCode, USHORT flags);
		};
	};
};
