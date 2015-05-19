#pragma once


//---------------------------INIT HEADER---------------------------
#ifndef _X39_Globals
#define _X39_Globals 1

#ifndef _DEBUG //Force DEBUG mode
#define _DEBUG 1
#endif



//Link required libaries
#ifndef _X39_INCLUDENEEDEDLIBARIES
	#define _X39_INCLUDENEEDEDLIBARIES 1

	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "glew32s.lib")

#endif
//Define Debug macros
#define _DEBUGRAW 1					//Output informations about RAW input (not processed)
#define _CAPTUREMOUSE 1				//Capture MouseInput
#define _CAPTUREKEYBOARD 1			//Capture KeyboardInput
#define ENABLE_LOGGER 1				//Enable logger
#define ENABLE_LOGGER_ToConsole 1	//Enable logging to console
//#define ENABLE_LOGGER_ToFile 1		//Enable logging to log.txt

#define STRINGIFY(DEF) #DEF

#define strEndsWith(STRFULL, STREND) STRFULL.compare(STRFULL.length() - strlen(STREND), strlen(STREND), STREND) == 0
#ifdef _DEBUG
	#include "startPoint.h"
	#define CHECKOPENGLERROR CheckForOpenGLErrors();
	#define CHECKFOROPENGLERROR(CODE) CODE; CheckForOpenGLErrors()
#else
	#define CHECKOPENGLERROR 
	#define CHECKFOROPENGLERROR(CODE) CODE
#endif

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif
#include "GL\glew.h"
#include "GL\wglew.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
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
#include "GlobalObject.h"
#include "FontManager.h"

#define TEXTUREUNITFONTS GL_TEXTURE0

#endif
//-----------------------------------------------------------------