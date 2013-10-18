#include "startPoint.h"
#include "EntityPlayer.h"//TODO: remove this! it was added because there was no way to do that live

Globals* g;
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
	LOGGER_START(Logger::INFO, "")
	g = new Globals;
	calculateExecutivePath();

#ifdef _DEBUG
	X39::Rendering::createConsoleWindow();
#endif
    g->hInstance = hInstance;
	g->pGameCameraInstance = new X39::EngineObjects::GameCamera();
	g->pKeyHandlerInstance = new X39::EngineObjects::KeyHandler();
	g->pMouseHandlerInstance = new X39::EngineObjects::MouseHandler();
	g->pRenderManagerInstance = new X39::Rendering::RenderManager();
	g->pEntityFactoryInstance = new X39::EntitySystem::EntityFactory(10);
	g->pModelManagerInstance = new X39::Rendering::ModelManager();
	pEntityFactory->createEntity(new X39::EntitySystem::EntityPlayer(), NULL);
	int i = -1;
	if(i = X39::Rendering::createWindow(hInstance, iCmdShow) != 0)
	{
		if(i == -1)
			FatalAppExit( NULL, TEXT("CreateWindow() failed!") );
	}
	if(i = X39::Rendering::setPixelFormat() != 0)
	{
		if(i == -1)
			FatalAppExit( NULL, TEXT("ChoosePixelFormat() failed!") );
		if(i == -2)
			FatalAppExit( NULL, TEXT("SetPixelFormat() failed!") );
	}
	if(i = X39::Rendering::createRenderingContext() != 0)
	{

	}
	if(i = X39::Rendering::createDevices() != 0)
	{
		DWORD d = GetLastError();
		printf("%s", GetLastError());
	}
	glewExperimental = GL_TRUE;
	GLenum init = glewInit();
	if(init != GLEW_OK)
	{
		return 1;
	}

    #pragma region message loop
    MSG msg;
	DWORD framerateHelper_last = GetTickCount();
    while( 1 )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT )
            {
                break;
            }
            
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			pRenderManager->draw();
			pEntityFactory->InitEntitySystem();
			{//Calculate framerate
				DWORD framerateHelper_new = GetTickCount();
				double framerate = (1 / ((double)(framerateHelper_new - framerateHelper_last) / 1000));

				//printf("[FRAMERATE]%i\n", (int)framerate);
				framerateHelper_last = framerateHelper_new;
				//Sleep();
			}
        }
    }
    #pragma endregion

	delete g->pRenderManagerInstance;
	delete g->pMouseHandlerInstance;
	delete g->pKeyHandlerInstance;
	delete g->pGameCameraInstance;
	delete g->pEntityFactoryInstance;
	delete g;
	LOGGER_STOP()
    return msg.wParam;
}

void calculateExecutivePath(void) {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
	std::string str = std::string( buffer ).substr( 0, pos + 1);
	g->executivePath = new char[str.length()];
	strcpy(g->executivePath, str.c_str());
}