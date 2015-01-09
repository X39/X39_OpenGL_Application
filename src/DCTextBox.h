#pragma once

#include "DisplayBase.h"
#include <Windows.h>
#include <string>

namespace X39
{
	namespace GUI
	{
		class DCTextBox : public DisplayBase
		{
			private:
				std::string innerText;

			public:
				DCTextBox(double x, double y, double w, double h);
				DCTextBox(double x, double y, double w, double h, std::string &s);
				~DCTextBox(void);
				
				std::string getInnerText(void);
				void setInnerText(std::string &s);

				virtual void e_draw(void);
				virtual bool e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
				virtual bool e_mouseMove(int posX, int posY, LPPOINT mousePos);
				virtual bool e_keyPressed(int mode, USHORT key);
		};
	};
};