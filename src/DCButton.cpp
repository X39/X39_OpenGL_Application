#include "DCButton.h"


namespace X39
{
	namespace GUI
	{
		DCButton::DCButton(double x, double y, double w, double h, void (*callback)(void), std::string& displayedText) : DisplayBase(x, y, w, h)
		{
			callbackFunction = callback;
			displayText = displayedText;
		}

		DCButton::~DCButton(void)
		{

		}

		void DCButton::e_draw()
		{
			::X39::Singletons::MATERIAL* mat = ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(7);
			drawTexture2D(mat, 0, 0, mat->textures[0].width, mat->textures[0].height, posX, posY, width, height);
			if(!displayText.empty())
				drawTextLine2D(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(3), displayText.length(), displayText.c_str(), posX, posY, width, height);
		}
		bool DCButton::e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			if(ulButtons != 2)
				return false;
			if(!isPointInDisplay(mousePos))
				return false;
			callbackFunction();
			return true;
		}
		bool DCButton::e_mouseMove(int posX, int posY, LPPOINT mousePos){return false;}
		bool DCButton::e_keyPressed(int mode, USHORT key){return false;}
	};
};