#pragma once

#include "globals.h"
#include "glHeaders.h"

#include "GameCamera.h"
#include "KeyHandler.h"
#include "MouseHandler.h"
#include "external\tgaLoader\tga.h"
#include "ModelManager.h"
#include "Model.h"

#include <Windows.h>
#include <WindowsX.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>



namespace X39
{
	namespace Rendering
	{
		int createWindow(HINSTANCE hInstance, int iCmdShow);
		int setPixelFormat();
		int createRenderingContext();
		int cleanup();
		int createDevices();
		LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
		int createConsoleWindow();
		void setStyleBorderless(void);
		void getScreenResolution(RECT* result);

		class RenderManager
		{
		public:
			RenderManager(void);
			~RenderManager(void);

			void draw(void);
			bool CheckForOpenGLErrors(void);
		};
		//RenderManager* pRenderManager;
	};
};