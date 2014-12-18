#pragma once


//---------------------------INIT HEADER---------------------------
#ifndef _X39_Globals
	#define _X39_Globals 1
#endif
//-----------------------------------------------------------------



//Include needed libaries
#ifndef _X39_INCLUDENEEDEDLIBARIES
	#define _X39_INCLUDENEEDEDLIBARIES 1

	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "glew32.lib")

#endif
//Define Debug macros
#ifndef _DEBUG //Manual DEBUG mode
	#define _DEBUG 1
#endif
#define _DEBUGRAW 1					//Output informations about RAW input (not processed)
#define _CAPTUREMOUSE 1				//Capture MouseInput
#define _CAPTUREKEYBOARD 1			//Capture KeyboardInput
#define ENABLE_LOGGER 1				//Enable log file
#define ENABLE_LOGGER_ToConsole 1	//Enable log file


#include "GL\glew.h"
#include "GL\wglew.h"
#include "glm\glm.hpp"
#include <gl/gl.h>
#include <gl/glu.h>

//OBSOLTE! will be removed soon
#include <windows.h>
#include <string>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
//define PreCalculated PI constant (to save time)
#ifndef PIconst
	#define PIconst 3.1415926535897932384626433832795
#endif
//include logger class
#include "Logger.h"

#include "GameCamera.h"
#include "Mouse.h"
#include "KeyHandler.h"
#include "MaterialManager.h"

#ifndef _GLOBALNAMESPACE_
	#define _GLOBALNAMESPACE_
	namespace GLOBAL
	{
		namespace RENDER
		{
			static int width;
			static int height;
		}
		static HDC windowDisplayContext;
		static HWND windowHandle;
		static HGLRC windowGlRenderingContextHandle;
		static LPTSTR executionPath;
	}
#endif