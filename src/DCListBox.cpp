#include "globals.h"
#include "DCListBox.h"
#include "KeyEventCodes.h"
#include "KeyHandler.h"


namespace X39
{
	namespace GUI
	{
		DCListBox::DCListBox(double x, double y, double w, double h) : DisplayBase(x, y, w, h)
		{
		}

		DCListBox::~DCListBox(void)
		{//ToDo: Implement
		}
		
		void DCListBox::e_draw()
		{

			::X39::Singletons::MATERIAL* mat = ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(1);
			int textureIndex = 1;
			getShader().use();
			if (width < 16 || height < 16)
			{
				drawTexture2D(mat, textureIndex, 0, 0, mat->textures[0]->width, mat->textures[0]->height, posX, posY, width, height, getShader());
			}
			else
			{
				//TopLeft corner
				drawTexture2D(mat, textureIndex, 0, 0, 16, 16, posX, posY, 16, 16, getShader());
				if (height > 32)
				{
					//Left Side
					drawTexture2D(mat, textureIndex, 0, 16, 16, 16, posX, posY + 16, 16, height - 32, getShader());
				}
				//BottomLeft corner
				drawTexture2D(mat, textureIndex, 0, 64 - 16, 16, 16, posX, posY + height - 16, 16, 16, getShader());

				//TopRight corner
				drawTexture2D(mat, textureIndex, 64 - 16, 0, 16, 16, posX + width - 16, posY, 16, 16, getShader());
				if (height > 32)
				{
					//Right Side
					drawTexture2D(mat, textureIndex, 64 - 16, 16, 16, 16, posX + width - 16, posY + 16, 16, height - 32, getShader());
				}
				//BottomRight corner
				drawTexture2D(mat, textureIndex, 64 - 16, 64 - 16, 16, 16, posX + width - 16, posY + height - 16, 16, 16, getShader());

				if (width > 32)
				{
					//Top side
					drawTexture2D(mat, textureIndex, 16, 0, 16, 16, posX + 16, posY, width - 32, 16, getShader());

					if (height > 32)
					{
						//Middle side
						drawTexture2D(mat, textureIndex, 16, 16, 16, 16, posX + 16, posY + 16, width - 32, height - 32, getShader());
					}
					//Bottom side
					drawTexture2D(mat, textureIndex, 16, 64 - 16, 16, 16, posX + 16, posY + height - 16, width - 32, 16, getShader());
				}
			}
			getShader().unuse();
			if (!innerText.empty())
				drawTextLine2D(::X39::Singletons::FontManager::getInstance().getFont(0), innerText.c_str(), height / FONTSIZEBASE, posX, posY, width);
		}
		bool DCListBox::e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			if (ulButtons != 2)
				return false;
			if (!isPointInDisplay(mousePos))
				return false;
			return true;
		}
		bool DCListBox::e_mouseMove(int posX, int posY, LPPOINT mousePos){ return false; }
		bool DCListBox::e_keyPressed(int mode, USHORT key)
		{
			if (!isFocused())
				return false;
			return false;
		}
	};
};