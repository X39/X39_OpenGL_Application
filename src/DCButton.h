#pragma once

#include "DisplayBase.h"
#include <Windows.h>
#include <string>


namespace X39
{
	namespace GUI
	{
		class DCButton : public DisplayBase
		{
			private:
				void (*callbackFunction)(void);
				std::string displayText;

			public:
				DCButton(double x, double y, double w, double h, void (*callback)(void), std::string& displayedText);
				~DCButton(void);

				virtual void e_draw(void);
				virtual bool e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
				virtual bool e_mouseMove(int posX, int posY, LPPOINT mousePos);
				virtual bool e_keyPressed(int mode, USHORT key);
		};
	};
};