#pragma once

#include "GuiBase.h"

#include <Windows.h>

namespace X39
{
	class GlobalObject
	{
		public:
			GlobalObject(void);
			~GlobalObject(void);
			static GlobalObject& getInstance(void);
		
			int render_width;
			int render_height;

			HDC windowDisplayContext;
			HWND windowHandle;
			HGLRC windowGlRenderingContextHandle;
			LPTSTR executionPath;

			::X39::GUI::GuiBase* mainDisplay;
	};
};