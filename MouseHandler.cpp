#include "MouseHandler.h"


namespace X39
{
	namespace EngineObjects
	{
		MouseHandler::MouseHandler(void)
		{
			sensitivy = 0.5;
			hasFocus = true;
			invertPitch = false;
		}

		MouseHandler::~MouseHandler(void)
		{
		}


		void MouseHandler::mouseMove(int posX, int posY)
		{
			if(hasFocus)
			{
#if _DEBUG
				printf("[Mouse]\tX:%i\tY:%i\n", posX, posY);
#endif
				//printf("%i", invertPitch);
				double yawModificator = posX * sensitivy ;
				double pitchModificator = posY * sensitivy;
				if(invertPitch)
				{
					pitchModificator *= -1;
				}
				if(yawModificator > 10)
				{
					yawModificator = 10;
				}
				if(pitchModificator > 10)
				{
					pitchModificator = 10;
				}
				pGameCamera->setYaw(yawModificator + pGameCamera->getYaw());
				pGameCamera->setPitch(pitchModificator + pGameCamera->getPitch());

				POINT pt = {g->renderHeight / 2, g->renderWidth / 2};
				ClientToScreen(g->hwnd, &pt);
				SetCursorPos(pt.x, pt.y);

				//skipNext = true;
			}//else if(skipNext)
			//{
			//	skipNext = false;
			//}
		}
		void MouseHandler::mouseButtonEvent(ULONG ulButtons, USHORT usButtonData)
		{

		}
		void MouseHandler::setFocus(bool flag)
		{
#if _DEBUG
				printf("[Mouse]\tSetting focus:%i\n", flag);
#endif
			hasFocus = flag;
			showCursor(!flag);
		}
		void MouseHandler::showCursor(bool flag)
		{
#if _DEBUG
				printf("[Mouse]\tSetting hideCursor:%i\n", !flag);
#endif
			ShowCursor(flag);
		}
	};
};
