#include "DCTextBox.h"
#include "KeyEventCodes.h"
#include "KeyHandler.h"


namespace X39
{
	namespace GUI
	{
		DCTextBox::DCTextBox(double x, double y, double w, double h) : DisplayBase(x, y, w, h)
		{
			innerText = std::string();
		}
		DCTextBox::DCTextBox(double x, double y, double w, double h, std::string &s) : DisplayBase(x, y, w, h)
		{
			innerText = s;
		}

		DCTextBox::~DCTextBox(void)
		{
		}
		
		void DCTextBox::e_draw()
		{
			::X39::Singletons::MATERIAL* mat = ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(7);
			drawTexture2D(mat, 0, 0, mat->textures[0].width, mat->textures[0].height, posX, posY, width, height);
			if(!innerText.empty())
				drawTextLine2D(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(3), innerText.c_str(), height / FONTSIZEBASE, posX, posY, width);
		}
		bool DCTextBox::e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			if(ulButtons != 2)
				return false;
			if(!isPointInDisplay(mousePos))
				return false;
			return true;
		}
		bool DCTextBox::e_mouseMove(int posX, int posY, LPPOINT mousePos){return false;}
		bool DCTextBox::e_keyPressed(int mode, USHORT key)
		{
			if(!isFocused())
				return false;
			if(mode != KEYEVENT_KEYDOWN)
				return false;
			char c = MapVirtualKey(MapVirtualKey(key, MAPVK_VSC_TO_VK), MAPVK_VK_TO_CHAR);
			if(c == 0x00)
				return false;
			bool shift = ::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LShift) || ::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_RShift);
			if(!shift && c >= 'A' && c <= 'Z')
				c += 'a' - 'A';
			//ToDo: find a better way to get correct key ...
			if(c == 0x08)
			{
				if(innerText.length() > 0)
					innerText.pop_back();
			}
			else
			{
				innerText.push_back(c);
			}
			return false;
		}
	};
};