#include "DisplayBase.h"


namespace X39
{
	namespace GUI
	{
		DisplayBase::DisplayBase(double x, double y, double w, double h) : GuiBase(true)
		{
			posX = x;
			posY = y;
			width = w;
			height = h;
		}
		DisplayBase::DisplayBase(double x, double y, double w, double h, bool focusable) : GuiBase(focusable)
		{
			posX = x;
			posY = y;
			width = w;
			height = h;
		}
		
		DisplayBase::~DisplayBase(void)
		{
		}
		
		double DisplayBase::left(void)		{ return posX; }
		double DisplayBase::right(void)		{ return posX + width; }
		double DisplayBase::top(void)		{ return posY; }
		double DisplayBase::bottom(void)	{ return posY + height; }
		bool DisplayBase::isPointInDisplay(LPPOINT p)	{ return (p->x >= left() && p->x <= right() && p->y >= top() && p->y <= bottom()); }



		
		bool DisplayBase::mouseClick(LPPOINT mousePos, ULONG ulButtons, USHORT usButtonData)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->mouseClick(mousePos, ulButtons, usButtonData))
					break;
			if(!this->isEnabled())
				return false;
			bool flag = isPointInDisplay(mousePos);
			if(flag && isFocusable)
				setFocused();
			return this->e_mouseClick(mousePos, ulButtons, usButtonData) || flag;
		}

		bool DisplayBase::mouseMove(int posX, int posY, LPPOINT mousePos)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->mouseMove(posX, posY, mousePos))
					break;
			if(!this->isEnabled())
				return false;
			return this->e_mouseMove(posX, posY, mousePos) || isPointInDisplay(mousePos);
		}

		bool DisplayBase::keyPressed(int mode, USHORT key)
		{
			for(int i = 0; i < this->children.size(); i++)
				if(this->children[i]->isEnabled() && this->children[i]->keyPressed(mode, key))
					break;
			if(!this->isEnabled())
				return false;
			return this->e_keyPressed(mode, key);
		}
	}
}