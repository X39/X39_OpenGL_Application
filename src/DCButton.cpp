#include "globals.h"
#include "DCButton.h"


namespace X39
{
	namespace GUI
	{
		DCButton::DCButton(double x, double y, double w, double h, void(*callback)(void), std::string& displayedText, ::X39::Singletons::MATERIAL& mat, unsigned int textureIndex) : DisplayBase(x, y, w, h), material(mat)
		{
			this->callbackFunction = callback;
			this->displayText = displayedText;
			this->textureIndex = textureIndex;
		}

		DCButton::~DCButton(void)
		{

		}

		void DCButton::e_draw()
		{
			::X39::Singletons::MATERIAL* mat = &this->material;
			int textureIndex = 0;
			getShader().use();
			if (width < 16 || height < 16)
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
			if(!displayText.empty())
				drawTextLine2D(::X39::Singletons::FontManager::getInstance().getFont(0), displayText.length(), displayText.c_str(), posX, posY, width, height, 0, 0, 0);
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