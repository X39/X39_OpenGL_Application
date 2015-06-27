#pragma once

#include "DisplayBase.h"
#include <Windows.h>
#include <string>

namespace X39
{
	namespace GUI
	{
		class DCListBox : public DisplayBase
		{
		private:
			std::string innerText;

		public:
			DCListBox(double x, double y, double w, double h);
			DCListBox(double x, double y, double w, double h, std::string &s);
			~DCListBox(void);

			std::string getInnerText(void);
			void setInnerText(std::string &s);

			virtual void e_draw(void);
			virtual bool e_mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
			virtual bool e_mouseMove(int posX, int posY, LPPOINT mousePos);
			virtual bool e_keyPressed(int mode, USHORT key);
		};
	};
};

