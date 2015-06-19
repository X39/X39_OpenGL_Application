#include "globals.h"
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
		static bool deleteButtonPressed = false;
		static unsigned int deleteButtonPressedTimeout = 0;
		void DCTextBox::e_draw()
		{
			
			::X39::Singletons::MATERIAL* mat = ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(1);
			int textureIndex = 1;
			getShader().use();
			if(width < 16 || height < 16)
			{
				drawTexture2D(mat, textureIndex, 0, 0, mat->textures[0]->width, mat->textures[0]->height, posX, posY, width, height, getShader());
			}
			else
			{
				//TopLeft corner
				drawTexture2D(mat, textureIndex, 0, 0, 16, 16, posX, posY, 16, 16, getShader());
				if(height > 32)
				{
					//Left Side
					drawTexture2D(mat, textureIndex, 0, 16, 16, 16, posX, posY + 16, 16, height - 32, getShader());
				}
				//BottomLeft corner
				drawTexture2D(mat, textureIndex, 0, 64 - 16, 16, 16, posX, posY + height - 16, 16, 16, getShader());

				//TopRight corner
				drawTexture2D(mat, textureIndex, 64 - 16, 0, 16, 16, posX + width - 16, posY, 16, 16, getShader());
				if(height > 32)
				{
					//Right Side
					drawTexture2D(mat, textureIndex, 64 - 16, 16, 16, 16, posX + width - 16, posY + 16, 16, height - 32, getShader());
				}
				//BottomRight corner
				drawTexture2D(mat, textureIndex, 64 - 16, 64 - 16, 16, 16, posX + width - 16, posY + height - 16, 16, 16, getShader());
				
				if(width > 32)
				{
					//Top side
					drawTexture2D(mat, textureIndex, 16, 0, 16, 16, posX + 16, posY, width - 32, 16, getShader());
					
					if(height > 32)
					{
						//Middle side
						drawTexture2D(mat, textureIndex, 16, 16, 16, 16, posX + 16, posY + 16, width - 32, height - 32, getShader());
					}
					//Bottom side
					drawTexture2D(mat, textureIndex, 16, 64 - 16, 16, 16, posX + 16, posY + height - 16, width - 32, 16, getShader());
				}
			}
			getShader().unuse();
			if(!innerText.empty())
				drawTextLine2D(::X39::Singletons::FontManager::getInstance().getFont(0), innerText.c_str(), height / FONTSIZEBASE, posX, posY, width);
			if(deleteButtonPressed)
			{
				deleteButtonPressedTimeout++;
				if(deleteButtonPressedTimeout > 25)
				{
					deleteButtonPressedTimeout = 20;
					if(innerText.size() > 0)
						innerText.pop_back();
				}
			}
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
			char c = MapVirtualKey(MapVirtualKey(key, MAPVK_VSC_TO_VK), MAPVK_VK_TO_CHAR);
			if(c == 0x00)
				return false;
			bool shift = ::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LShift) || ::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_RShift);
			if(!shift && c >= 'A' && c <= 'Z')
				c += 'a' - 'A';
			//ToDo: find a better way to get correct key ...
			if(c == 0x08)
			{
				deleteButtonPressed = mode == KEYEVENT_KEYDOWN;
				deleteButtonPressedTimeout = 0;
				if(deleteButtonPressed)
					if(innerText.size() > 0)
						innerText.pop_back();
			}
			else
			{
				if(mode != KEYEVENT_KEYDOWN)
					return false;
				innerText.push_back(c);
			}
			return false;
		}
	};
};