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
#define _DEBUGRAW 0					//Output informations about RAW input (not processed)
#define _CAPTUREMOUSE 1				//Capture MouseInput
#define _CAPTUREKEYBOARD 1			//Capture KeyboardInput
#define ENABLE_LOGGER 1				//Enable log file
#define ENABLE_LOGGER_ToConsole 1	//Enable log file


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

#define pRenderManager ((::X39::Rendering::RenderManager*) g->pRenderManagerInstance)
#define pGameCamera ((::X39::EngineObjects::GameCamera*) g->pGameCameraInstance)
#define pKeyHandler ((::X39::EngineObjects::KeyHandler*) g->pKeyHandlerInstance)
#define pMouseHandler ((::X39::EngineObjects::MouseHandler*) g->pMouseHandlerInstance)
#define pEntityFactory ((::X39::EntitySystem::EntityFactory*) g->pEntityFactoryInstance)
#define pModelManager ((::X39::Rendering::ModelManager*) g->pModelManagerInstance)



struct Globals
{
    HINSTANCE hInstance;    // window app instance

    HWND hwnd;      // handle for the window

    HDC   hdc;      // handle to device context

    HGLRC hglrc;    // handle to OpenGL rendering context
	char* executivePath;
	int renderWidth;
	int renderHeight;

	void* pRenderManagerInstance;
	void* pGameCameraInstance;
	void* pKeyHandlerInstance;
	void* pMouseHandlerInstance;
	void* pEntityFactoryInstance;
	void* pModelManagerInstance;

};
extern Globals* g;