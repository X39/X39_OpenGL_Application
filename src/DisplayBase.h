#pragma once

#include "GuiBase.h"
#include <Windows.h>

namespace X39
{
	namespace GUI
	{
		class DisplayBase: public GuiBase
		{
			protected:
				double posX;
				double posY;
				double width;
				double height;

			public:
				DisplayBase(double x, double y, double w, double h);
				DisplayBase(double x, double y, double w, double h, bool focusable);
				~DisplayBase(void);
				
				
				double left(void);
				double top(void);
				double bottom(void);
				double right(void);

				bool isPointInDisplay(LPPOINT p);

				virtual bool mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData);
				virtual bool mouseMove(int posX, int posY, LPPOINT mousePos);
				virtual bool keyPressed(int mode, USHORT key);
		};
	};
};