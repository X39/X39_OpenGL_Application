#include "startPoint.h"
#include "globals.h"
#include "KeyEventCodes.h"
#include "DCButton.h"
#include "DCTextBox.h"
#include "Shader.h"
#include "Model.h"


#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <omp.h>
#include <array>

LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) 
{
	switch( message )
	{
//	case WM_CREATE:
//		Beep( 50, 10 );
//		return 0;
//		break;
	case WM_DESTROY:
		PostQuitMessage( 0 ) ;
		return 0;
		break;
//	case WM_KILLFOCUS:
//		break;
	case WM_SETFOCUS:
		::X39::Singletons::Mouse::getInstance().keepCenter();
		break;
//	case WM_MOUSEMOVE://http://msdn.microsoft.com/en-us/library/windows/desktop/ms645616(v=vs.85).aspx
//		pMouseHandler->mouseMove(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
//		//printf("[Mouse]\tX:%i\tY:%i\n", GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
//		break;
//	case WM_KEYDOWN:
//		::X39::Singletons::KeyHandler::getInstance().KeyEvent(wparam);
//		return 0;
//	}
//	case WM_ENTERSIZEMOVE:
//		//pMouseHandler->setFocus(false);
//		return TRUE;
//		break;
//	case WM_EXITSIZEMOVE:
//		//pMouseHandler->setFocus(true);
//		return 0;
//		break;
//	case WM_PAINT:
//		{
//			HDC hdc;
//			PAINTSTRUCT ps;
//			hdc = BeginPaint( hwnd, &ps );
//				// don't draw here.  would be waaay too slow.
//				// draw in the draw() function instead.
//			EndPaint( hwnd, &ps );
//		}
//		return 0;
//		break;
	case WM_INPUT: 
		{
			UINT dwSize;

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == NULL) 
			{
				return 0;
			} 

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
				LOGGER_WRITE(::Logger::ERRORmsg, "GetRawInputData does not return correct size !\n"); 

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEKEYBOARD) 
			{
				if(raw->data.keyboard.MakeCode == ::EngineKeySet::KEY_F1)
					exit(0);
#if _CAPTUREKEYBOARD
	#if _DEBUGRAW
				char s[32];
				
				GetKeyNameTextA(raw->data.keyboard.VKey, s, 32);
				char str[256];
				sprintf(str, "[Keyboard]\t%i, %i, %i, %i, %i, %i\t%s%i",	
					raw->data.keyboard.MakeCode, 
					raw->data.keyboard.Flags, 
					raw->data.keyboard.Reserved, 
					raw->data.keyboard.ExtraInformation, 
					raw->data.keyboard.Message, 
					raw->data.keyboard.VKey, s, GetLastError());
				LOGGER_WRITE(::Logger::USERINPUT, str);
	#endif
						::X39::Singletons::KeyHandler::getInstance().handleRawKey(raw->data.keyboard.MakeCode, raw->data.keyboard.Flags);
#endif
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE) 
			{
#if _CAPTUREMOUSE
	#if _DEBUGRAW
				char str[256];
				sprintf(str, "[Mouse]\t%i, %i, %i, %i, %i, %i, %i, %i",	
					raw->data.mouse.usFlags, 
					raw->data.mouse.ulButtons, 
					raw->data.mouse.usButtonFlags, 
					raw->data.mouse.usButtonData, 
					raw->data.mouse.ulRawButtons, 
					raw->data.mouse.lLastX, 
					raw->data.mouse.lLastY, 
					raw->data.mouse.ulExtraInformation);
				LOGGER_WRITE(::Logger::USERINPUT, str);
	#endif
				LONG mouseX = raw->data.mouse.lLastX;
				LONG mouseY = raw->data.mouse.lLastY;
				if(mouseX != 0 || mouseY != 0)
				{
					::X39::Singletons::Mouse::getInstance().handleMouseMove(mouseX, mouseY);
				}
				else
				{
					::X39::Singletons::Mouse::getInstance().handleMouseButtonEvent(raw->data.mouse.ulButtons, raw->data.mouse.usButtonData);
				}
#endif
			} 
			delete[] lpb; 
			return 0;
		}
	}
	return DefWindowProc( hwnd, message, wparam, lparam );
}
int createWindow(HINSTANCE hInstance, int iCmdShow)
{
	
	/*
	The next few lines you should already
	be used to:  create a WNDCLASS
	that describes the properties of
	the window we're going to soon create.
	A.  Create the WNDCLASS
	*/
	WNDCLASS wc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );         
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );     
	wc.hInstance = hInstance;         
	wc.lpfnWndProc = WndProc;         
	wc.lpszClassName = TEXT("X39");
	wc.lpszMenuName = 0; 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClass(&wc);

	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	rect.top += 40;
	rect.left += 20;
	rect.right /= 2;
	rect.bottom /= 2;
	SetRect( &rect, rect.left  ,
					rect.top   ,
					rect.right ,
					rect.bottom);

	// Save width and height off.

	// Adjust it.
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );
	::X39::GlobalObject::getInstance().windowHandle = CreateWindow(TEXT("X39"),
						  TEXT("EngineTest"),
						  //WS_BORDER,
						  0,
						  rect.left, rect.top,  // adjusted x, y positions
						  rect.right, rect.bottom,  // adjusted width and height
						  NULL, NULL,
						  hInstance, NULL);
	SetWindowLongPtr(::X39::GlobalObject::getInstance().windowHandle, GWL_STYLE, GetWindowLongPtr(::X39::GlobalObject::getInstance().windowHandle, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU));
	SetWindowPos(::X39::GlobalObject::getInstance().windowHandle, NULL, 0, 0, -1, -1, SWP_NOSIZE);
	//Needs to be done everytime the window gets resized!
	if(GetWindowRect(::X39::GlobalObject::getInstance().windowHandle, &rect))
	{
		::X39::GlobalObject::getInstance().render_width = rect.right;
		::X39::GlobalObject::getInstance().render_height = rect.bottom;
	}
	else
	{
		//ERROR CODE FE001
		MessageBox(::X39::GlobalObject::getInstance().windowHandle, (LPCWSTR)L"Could not get window size\nFatal Error 001", (LPCWSTR)L"FATAL ERROR - FE001", MB_OK | MB_ICONERROR);
		exit(-1);
	}
	//setStyleBorderless();

	// check to see that the window
	// was created successfully!
	if(::X39::GlobalObject::getInstance().windowHandle == NULL)
	{
		//FatalAppExit( NULL, TEXT("CreateWindow() failed!") );
		return -1;
	}

	// and show.
	ShowWindow(::X39::GlobalObject::getInstance().windowHandle, iCmdShow);
	return 0;
}
void resizeWindow(int posX, int posY, int width, int height)
{
	UINT uFlags = 0;
	RECT rect;
	if(posX < 0 || posX < 0)
		uFlags |= SWP_NOREPOSITION;
	if(width < 0 || height < 0)
		uFlags |= SWP_NOSIZE;

	SetWindowPos(::X39::GlobalObject::getInstance().windowHandle, NULL, posX, posY, width, height, uFlags);
	//Needs to be done everytime the window gets resized!
	if(GetWindowRect(::X39::GlobalObject::getInstance().windowHandle, &rect))
	{
		::X39::GlobalObject::getInstance().render_width = rect.right;
		::X39::GlobalObject::getInstance().render_height = rect.bottom;
	}
	else
	{
		//ERROR CODE FE001
		MessageBox(::X39::GlobalObject::getInstance().windowHandle, (LPCWSTR)L"Could not get window size\nFatal Error 001", (LPCWSTR)L"FATAL ERROR - FE001", MB_OK | MB_ICONERROR);
		exit(-1);
	}
}
RECT GetDesktopResolution()
{
	RECT rect;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &rect);
	return rect;
}
int createConsoleWindow()
{
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

	SetWindowPos(GetConsoleWindow(), NULL, 1024, 200, -1, -1, SWP_NOACTIVATE | SWP_NOSIZE);

	return 0;
}
int setPixelFormat()
{
	/*
	2.  GET DC OF WINDOW, and keep it in our global
	    variable g->  We will NOT release this DC
	    until our app is about to exit.
	*/
	::X39::GlobalObject::getInstance().handleDeviceContext = GetDC( ::X39::GlobalObject::getInstance().windowHandle );
	/*
	If this keeping-DC-for-life-of-program-thing
	disturbs you as much as it disturbed me,
	GOOD LUCK in finding MS-based documentation to validate
	this practice!  All examples I've seen and
	Astle's OpenGL gaming book do it this way,
	so. . . I suppose its ok.
	
	One of the things I make sure to do is to specify
	CS_OWNDC when I create the window,
	so that Windows maintains a separate device context
	for my application's window.
	
	I haven't run into problems with this.  I don't
	think you should either.
	*/
	/*
	3.  SET PIXEL FORMAT OF HDC.
	
	We now have to set up the PIXELFORMAT
	of our HDC.

	A PIXEL FORMAT just describes the
	"qualities" that each pixel in the 
	window will have.  Do you want your
	OpenGL app to use 24 bit color
	("true color" -- really high
	quality!)?  Or 16 bit color (can look
	a bit washed out)?

	There are 3 substeps here:
	   A> create the PFD and set it up to describe
	      the pixel format we DESIRE (dream of having!)
	
	   B> call ChoosePixelFormat() to make windows
	      choose us the ID of the appropriate pixel format that
	      is CLOSEST to our dream pixel format.
	
	   C> Call SetPixelFormat() using the integer ID number
	      that ChoosePixelFormat() returned to us in step B>

	So let's do that:
	*/
	// A> CREATE PFD:
	PIXELFORMATDESCRIPTOR pfd = { 0 };  // create the pfd,
	/*
	and start it out with ALL ZEROs in ALL
	of its fields.

	A good description of the PIXELFORMATDESCRIPTOR
	struct is under the documentation
	for the ChoosePixelFormat() function:
	http://msdn2.microsoft.com/en-us/library/ms537556(VS.85).aspx

	If you look at the docs, MANY of the fields
	are "NOT USED" and REMAIN 0.

	That should be something of a relief to you!
	Look at the number of fields in this beast!

	So we set only the fields of the pfd we care about:
	*/
	pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );    // just its size
	pfd.nVersion = 1;   // always 1

	pfd.dwFlags = PFD_SUPPORT_OPENGL |  // OpenGL support - not DirectDraw
				  PFD_DOUBLEBUFFER   |  // double buffering support
				  PFD_DRAW_TO_WINDOW;   // draw to the app window, not to a bitmap image

	pfd.iPixelType = PFD_TYPE_RGBA ;    // red, green, blue, alpha for each pixel
	pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for green, 8 for blue.
										// This count of color bits EXCLUDES alpha.
	// 32 bits to measure pixel depth.  That's accurate!
	pfd.cDepthBits = 32;                

	
	/*
	B) Alright!  We've filled out the pfd
	and it describes the way we want
	our pixels to appear on the screen.
	
	Now this next step is a little bit weird.
	The thing is, there are only a couple of
	dozen ACCEPTABLE pixel formats in existence.
	
	In other words, the system MIGHT NOT
	be able to use a pixel format the likes
	of which you have described in your
	PIXELFORMATDESCRIPTOR.

	What to do?? It would be awful annoying
	to have to keep TRYING different
	PIXELFORMATDESCRIPTORS until we found
	one that actually WORKED on this system.

	So MSFT has a better solution.

	We'll make a call to a function called
	ChoosePixelFormat().  ChoosePixelFormat()
	will examine the PIXELFORMATDESCRIPTOR
	structure that you send it, then it will
	give you back an ID for the pixel format
	that MOST CLOSELY MATCHES the pixel format you
	SAID you wanted.
	*/
	
	int chosenPixelFormat = ChoosePixelFormat( ::X39::GlobalObject::getInstance().handleDeviceContext, &pfd );
	/*
	what comes back from ChoosePixelFormat() is
	an integer identifier for a specific pixel
	format that Windows ALREADY knows about.
	If you got 0 back, then there was an error.
	*/
	if( chosenPixelFormat == 0 )
	{
		//FatalAppExit( NULL, TEXT("ChoosePixelFormat() failed!") );
		return -1;
	}

	/*
	// C> So finally, we call SetPixelFormat() using the integer ID number
	// that ChoosePixelFormat() returned to us previously.
	*/
	int result = SetPixelFormat( ::X39::GlobalObject::getInstance().handleDeviceContext, chosenPixelFormat, &pfd );

	if (result == NULL)
	{
		//FatalAppExit( NULL, TEXT("SetPixelFormat() failed!") );
		return -2;
	}
	/*
	// and that's all there is to setting
	// the pixel format!
	*/
	return 0;
}
int createRenderingContext()
{
	/*
	4.  CREATE RENDERING CONTEXT (HGLRC).

	What's a rendering context?
	Its the "surface" that OpenGL
	will DRAW to.

	The HGLRC will be created
	such that it is COMPATIBLE
	with the hdc.
	*/
	::X39::GlobalObject::getInstance().windowGlRenderingContextHandle = wglCreateContext( ::X39::GlobalObject::getInstance().handleDeviceContext );
	/*
	Created the rendering context
	and saved handle to it in global 'g'.
	
	Wasn't that awfully easy to create
	such a complicated sounding thing?
	*/
	/*
	5.  CONNECT THE RENDER CONTEXT (HGLRC)
	    WITH THE DEVICE CONTEXT (HDC) OF WINDOW.
	*/
	wglMakeCurrent( ::X39::GlobalObject::getInstance().handleDeviceContext, ::X39::GlobalObject::getInstance().windowGlRenderingContextHandle );
	/*
	
	OPEN GL INIT COMPLETED!!
	*/
	return 0;
}
int createDevices()
{
	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01; 
	Rid[0].usUsage = 0x02;				// adds HID mouse
	Rid[0].dwFlags = 0x00;	// ignores legacy mouse messages
	Rid[0].hwndTarget = ::X39::GlobalObject::getInstance().windowHandle;

	Rid[1].usUsagePage = 0x01; 
	Rid[1].usUsage = 0x06;				// adds HID keyboard
	Rid[1].dwFlags = RIDEV_NOLEGACY;	//ignores legacy keyboard messages
	Rid[1].hwndTarget = ::X39::GlobalObject::getInstance().windowHandle;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		return -1;
	}
	return 0;
}

bool CheckForOpenGLErrors(void)
{
	int i;
	bool flag = false;
	std::string errorString = "[GLError] got following errors: \n";
	while((i = glGetError()) != GL_NO_ERROR)
	{
		flag = true;
		switch(i)
		{

		case GL_INVALID_ENUM:
			errorString.append("\t-GL_INVALID_ENUM\n");
			break;
		
		case GL_INVALID_VALUE:
			errorString.append("\t-GL_INVALID_VALUE\n");
			break;
		
		case GL_INVALID_OPERATION:
			errorString.append("\t-GL_INVALID_OPERATION\n");
			break;
		
		case GL_OUT_OF_MEMORY:
			errorString.append("\t-GL_OUT_OF_MEMORY\n");
			break;
		
		default:
			errorString.append("\t-UNKNOWN[");
			errorString.append((char*) &i);
			errorString.append("]\n");
			break;
		}
	}
	if(flag)
	{
		LOGGER_WRITE(::Logger::GL_ERROR, errorString.c_str());
	}
	return flag;
}
bool debugSwitchMouseMode(int mode, USHORT key)
{
	if(mode == KEYEVENT_KEYDOWN && (key == ::EngineKeySet::KEY_F9 || key == ::EngineKeySet::KEY_F10))
		::X39::Singletons::Mouse::getInstance().setMode(::X39::Singletons::Mouse::getInstance().getMode() == MOUSEMODE_GAMECAMERA ? MOUSEMODE_MENU : MOUSEMODE_GAMECAMERA);
	return false;
}
void debugButtonTest(void)
{
	MessageBox(::X39::GlobalObject::getInstance().windowHandle, TEXT("TEST"), TEXT("CAPTION"), 0);
}
bool doDisplayKeyHandling(int mode, USHORT key)
{
	return ::X39::GlobalObject::getInstance().mainDisplay->keyPressed(mode, key);
}
bool doDisplayMouseClickHandling(LPPOINT point, ULONG ulButtons, USHORT usButtonData)
{
	return ::X39::GlobalObject::getInstance().mainDisplay->mouseClick(point, ulButtons, usButtonData);
}
bool doDisplayMouseMoveHandling(int posX, int posY, LPPOINT mousePos)
{
	return ::X39::GlobalObject::getInstance().mainDisplay->mouseMove(posX, posY, mousePos);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	LOGGER_START(::Logger::CONFIG, "log.txt")
#ifdef _DEBUG
	createConsoleWindow();
#endif
	int i = -1;
	wchar_t bufferStr[256];
	if(i = GetCurrentDirectory(256, bufferStr) < 0)
		FatalAppExit(NULL, TEXT("failed to receive execution path!"));
	::X39::GlobalObject::getInstance().executionPath = bufferStr;
	if(i = createWindow(hInstance, iCmdShow) != 0)
	{
		if(i == -1)
			FatalAppExit(NULL, TEXT("CreateWindow() failed!"));
	}
	if(i = setPixelFormat() != 0)
	{
		if(i == -1)
			FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));
		if(i == -2)
			FatalAppExit(NULL, TEXT("SetPixelFormat() failed!"));
	}
	if(i = createRenderingContext() != 0)
	{
			FatalAppExit(NULL, TEXT("createRenderingContext() failed!"));
	}
	if(i = createDevices() != 0)
	{
		DWORD d = GetLastError();
		LOGGER_WRITE(::Logger::Priority::INFO, d);
	}
	glewExperimental = GL_TRUE;
	GLenum init = glewInit();
	if(init != GLEW_OK)
	{
		return 1;
	}

	::X39::GlobalObject::getInstance().mainDisplay = new ::X39::GUI::GuiBase();
	
	//RECT rect = GetDesktopResolution();
	//resizeWindow(0, 0, rect.right, rect.bottom);

    #pragma region message loop
    MSG msg;
	DWORD framerateHelper_last = GetTickCount();
	double frameTime = 0;
	int tickOut = 0;

	
	::X39::Singletons::KeyHandler::getInstance().registerEventHandler(doDisplayKeyHandling);
	::X39::Singletons::Mouse::getInstance().registerEventHandler(doDisplayMouseClickHandling);
	::X39::Singletons::Mouse::getInstance().registerEventHandler(doDisplayMouseMoveHandling);
	
	//Register important testing keys
	::X39::Singletons::KeyHandler::getInstance().registerEventHandler(debugSwitchMouseMode);
	//Preload test materials
	::X39::Singletons::MaterialManager::getInstance().registerTexture("Materials\\grass.vmat");
	::X39::Singletons::MaterialManager::getInstance().registerTexture("Materials\\ui_base\\ui_base.vmat");
	::X39::Singletons::FontManager::getInstance().registerFont("Fonts\\arial.ttf");
	::X39::Shader shad;
	shad.load("Shaders\\base.shad");
	shad.compile();
	::X39::Model model = ::X39::Model("Models\\test.obj");
	model.loader_dotObj();
	
	::X39::GlobalObject::getInstance().mainDisplay->addChild(new ::X39::GUI::DCButton(0, ::X39::GlobalObject::getInstance().render_height - 200, 160, 20, debugButtonTest, std::string("testButton")));
	::X39::GlobalObject::getInstance().mainDisplay->addChild(new ::X39::GUI::DCTextBox(0, ::X39::GlobalObject::getInstance().render_height - 40, ::X39::GlobalObject::getInstance().render_width / 2, 20, std::string()));
	//::X39::Singletons::GameCamera::getInstance().setViewVec(glm::vec3(0.67156154, -0.35836795, 0.64851946));
	//::X39::Singletons::GameCamera::getInstance().setPos(glm::vec3(-3.5397613, 6.5228815, -3.5409057));
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	GLuint vertBufferID, indexBufferID;
	glGenBuffers(1, &vertBufferID);
	glGenBuffers(1, &indexBufferID);

	struct Vec2 { float x, y; };
	struct Vec3 { float x, y, z; };
	struct Vert { Vec3 pos; Vec2 tex; };

	std::array<Vert, 8> cubeVerts = {{
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f } }, { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f } }, { {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } }, { { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f } }, { { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f } }
	}};

	std::array<unsigned int, 36> cubeIdxs = {{ 
		0, 2, 1, 0, 3, 2, // Right
		4, 5, 6, 4, 6, 7, // Left
		0, 7, 3, 0, 4, 7, // Top
		1, 6, 2, 1, 5, 6, // Bottom
		0, 5, 1, 0, 4, 5, // Front
		3, 7, 6, 3, 6, 2  // Back
	}};

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // Matches layout (location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
	glEnableVertexAttribArray(1); // Matches layout (location = 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
	//start endless loop
    while(true)
    {
		frameTime = omp_get_wtime();
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
#pragma region CameraMovement
			::glm::vec3 vec = ::X39::Singletons::GameCamera::getInstance().getPos();
			float pitch = (float)::X39::Singletons::GameCamera::getInstance().getPitch();
			float yaw = (float)::X39::Singletons::GameCamera::getInstance().getYaw();
			float modificator = 0.1F;
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LShift))
				modificator = 1;

			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_W))
			{
				float rPitch = pitch / 180 * (float)PIconst;
				float rYaw = yaw / 180 * (float)PIconst;
				vec.x += (cos(rYaw) * sin(rPitch)) * modificator;
				vec.y += (cos(rPitch)) * modificator;
				vec.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_A))
			{
				float rPitch = 90.0F / 180 * (float)PIconst;
				float rYaw = (yaw - 90) / 180 * (float)PIconst;
				vec.x += (cos(rYaw) * sin(rPitch)) * modificator;
				vec.y += (cos(rPitch)) * modificator;
				vec.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_S))
			{
				float rPitch = pitch / 180 * (float)PIconst;
				float rYaw = yaw / 180 * (float)PIconst;
				vec.x -= (cos(rYaw) * sin(rPitch)) * modificator;
				vec.y -= (cos(rPitch)) * modificator;
				vec.z -= (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_D))
			{
				float rPitch = 90.0F / 180 * (float)PIconst;
				float rYaw = (yaw + 90) / 180 * (float)PIconst;
				vec.x += (cos(rYaw) * sin(rPitch)) * modificator;
				vec.y += (cos(rPitch)) * modificator;
				vec.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_Spacebar))
			{
				vec.y += 1 * modificator;
			}
			if(::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LCTRL))
			{
				vec.y -= 1 * modificator;
			}
			::X39::Singletons::GameCamera::getInstance().setPos(vec);
#pragma endregion
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glViewport(0, 0, ::X39::GlobalObject::getInstance().render_width, ::X39::GlobalObject::getInstance().render_height);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_COORD_ARRAY);
			//glEnable(GL_CULL_FACE);
			//glFrontFace(GL_CCW);
			//glCullFace(GL_BACK);
			//glEnable(GL_BLEND);
			glEnable (GL_POINT_SMOOTH);
			glEnable (GL_LINE_SMOOTH);
			//http://wiki.delphigl.com/index.php/glBlendFunc for different examples
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

			glm::mat4 viewMatrix = X39::Singletons::GameCamera::getInstance().invokeGluLookAt();
			//glm::mat4 porjectionMatrix = glm::perspective((float)45.0,(float)::X39::GlobalObject::getInstance().render_width/(float)::X39::GlobalObject::getInstance().render_height, (float)1, (float)10000);
			glm::mat4 porjectionMatrix = glm::perspective(45.0f, (float)::X39::GlobalObject::getInstance().render_width/(float)::X39::GlobalObject::getInstance().render_height, 0.1f, 100.0f);
			GLdouble projection[16];
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			glPushMatrix();
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0));
			shad.use();
			for(int i = 0; i < 100; i++)
			{
				for(int j = 0; j < 100; j++)
				{
					//glPushMatrix();
					//glTranslated(i - X39::Singletons::GameCamera::getInstance().getPos().x, 0 - X39::Singletons::GameCamera::getInstance().getPos().y, j - X39::Singletons::GameCamera::getInstance().getPos().z);
					//shad.setUniformMatrix4fv(
					//	"modelMatrix",
					//	1,
					//	GL_FALSE,
					//	&glm::mat4( (float)i - X39::Singletons::GameCamera::getInstance().getPos().x, 
					//				(float)0 - X39::Singletons::GameCamera::getInstance().getPos().y,
					//				(float)j - X39::Singletons::GameCamera::getInstance().getPos().z,
					//				(float)1)[0][0],
					//	-1
					//);
					shad.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, &glm::mat4()[0][0], -1);
					shad.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, &viewMatrix[0][0], -1);
					shad.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &porjectionMatrix[0][0], -1);
					//shad.setUniformMatrix4fv("matrix", 1, GL_FALSE, &(porjectionMatrix * viewMatrix * glm::mat4(1.0f))[0][0], -1);
					//glBegin(GL_QUADS);
					//	glTexCoord2f(0, 1);	glVertex3f(0, 0, 100);
					//	glTexCoord2f(1,	1);	glVertex3f(100, 0, 100);
					//	glTexCoord2f(1,	0);	glVertex3f(100, 0, 0);
					//	glTexCoord2f(0, 0);	glVertex3f(0, 0, 0);
					//glEnd();
					glBindVertexArray(vaoID);
					glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
					glBindVertexArray(0);
					//glPopMatrix();
				}
			}
			shad.unuse();
			glPopMatrix();

			//2D projection
			//glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0, ::X39::GlobalObject::getInstance().render_width, ::X39::GlobalObject::getInstance().render_height, 0.0, -1.0, 10.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glDisable(GL_CULL_FACE);
			glClear(GL_DEPTH_BUFFER_BIT);
			glm::vec3 camView = ::X39::Singletons::GameCamera::getInstance().getViewVec();
			glm::vec3 camPos = ::X39::Singletons::GameCamera::getInstance().getPos();
			char s[256];
			sprintf(s, "POS: %lf, %lf, %lf\nVIEW: %lf, %lf, %lf", camPos.x, camPos.y, camPos.z, camView.x, camView.y, camView.z);
			//::X39::GUI::GuiBase::drawText2D(::X39::Singletons::FontManager::getInstance().getFont(0), s, 1, 0, 0);
			
			//::X39::GlobalObject::getInstance().mainDisplay->draw();
			//::X39::GUI::GuiBase::drawText2D(::X39::Singletons::FontManager::getInstance().getFont(0), "a-_.", 3, 0, 0);
			//glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			//glMatrixMode(GL_MODELVIEW);

			SwapBuffers(::X39::GlobalObject::getInstance().handleDeviceContext);
			CheckForOpenGLErrors();
        }
		frameTime = omp_get_wtime() - frameTime;
		if(tickOut > 1 / frameTime)
		{
			tickOut = 0;
			LOGGER_WRITE(::Logger::FPS, 1 / frameTime);
		}
		tickOut++;
    }
    #pragma endregion

    return msg.wParam;
}