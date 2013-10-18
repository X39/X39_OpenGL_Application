#include "RenderManager.h"

namespace X39
{
	namespace Rendering
	{


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
			getScreenResolution(&rect);
			rect.top += 40;
			rect.left += 20;
			rect.right /= 2;
			rect.bottom /= 2;
			SetRect( &rect, rect.left  ,
							rect.top   ,
							rect.right ,
							rect.bottom);
    
			// Save width and height off.
			g->renderWidth = rect.right - rect.left;
			g->renderHeight = rect.bottom - rect.top;
    
			// Adjust it.
			AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );
			g->hwnd = CreateWindow(TEXT("X39"),
								  TEXT("EngineTest"),
								  //WS_BORDER,
								  0,
								  rect.left, rect.top,  // adjusted x, y positions
								  rect.right, rect.bottom,  // adjusted width and height
								  NULL, NULL,
								  hInstance, NULL);
			//setStyleBorderless();

			// check to see that the window
			// was created successfully!
			if( g->hwnd == NULL )
			{
				//FatalAppExit( NULL, TEXT("CreateWindow() failed!") );
				return -1;
			}

			// and show.
			ShowWindow( g->hwnd, iCmdShow );
			return 0;
		}
		int setPixelFormat()
		{
			/*
			2.  GET DC OF WINDOW, and keep it in our global
			    variable g->  We will NOT release this DC
			    until our app is about to exit.
			*/
			g->hdc = GetDC( g->hwnd );
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
			
			int chosenPixelFormat = ChoosePixelFormat( g->hdc, &pfd );
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
			int result = SetPixelFormat( g->hdc, chosenPixelFormat, &pfd );

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
			g->hglrc = wglCreateContext( g->hdc );
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
			wglMakeCurrent( g->hdc, g->hglrc );
			/*
			
			OPEN GL INIT COMPLETED!!
			*/
			return 0;
		}
		int cleanup()
		{
			// UNmake your rendering context (make it 'uncurrent')
			wglMakeCurrent( NULL, NULL );

			// Delete the rendering context, we no longer need it.
			wglDeleteContext( g->hglrc );

			// release your window's DC
			ReleaseDC( g->hwnd, g->hdc );
			return 0;
		}
		int createDevices()
		{
			RAWINPUTDEVICE Rid[2];
        
			Rid[0].usUsagePage = 0x01; 
			Rid[0].usUsage = 0x02;				// adds HID mouse
			Rid[0].dwFlags = 0x00;	// ignores legacy mouse messages
			Rid[0].hwndTarget = g->hwnd;

			Rid[1].usUsagePage = 0x01; 
			Rid[1].usUsage = 0x06;				// adds HID keyboard
			Rid[1].dwFlags = RIDEV_NOLEGACY;	//ignores legacy keyboard messages
			Rid[1].hwndTarget = g->hwnd;

			if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
				return -1;
			}
			return 0;
		}
		LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) 
		{
			switch( message )
			{
			case WM_CREATE:
				Beep( 50, 10 );
				return 0;
				break;
			case WM_DESTROY:
				PostQuitMessage( 0 ) ;
				return 0;
				break;

			case WM_KILLFOCUS:
				pMouseHandler->setFocus(false);
				break;
			case WM_SETFOCUS:
				pMouseHandler->setFocus(true);
				break;
//			case WM_MOUSEMOVE://http://msdn.microsoft.com/en-us/library/windows/desktop/ms645616(v=vs.85).aspx
//				pMouseHandler->mouseMove(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
//				//printf("[Mouse]\tX:%i\tY:%i\n", GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
//				break;
//			case WM_KEYDOWN:
//				pKeyHandler->KeyEvent(wparam);
//				return 0;
//			}
			case WM_ENTERSIZEMOVE:
				pMouseHandler->setFocus(false);
				return TRUE;
				break;
			case WM_EXITSIZEMOVE:
				pMouseHandler->setFocus(true);
				return 0;
				break;
			case WM_PAINT:
				{
					HDC hdc;
					PAINTSTRUCT ps;
					hdc = BeginPaint( hwnd, &ps );
						// don't draw here.  would be waaay too slow.
						// draw in the draw() function instead.
					EndPaint( hwnd, &ps );
				}
				return 0;
				break;

			case WM_INPUT: 
				{
					UINT dwSize;

					GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, 
									sizeof(RAWINPUTHEADER));
					LPBYTE lpb = new BYTE[dwSize];
					if (lpb == NULL) 
					{
						return 0;
					} 

					if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, 
						 sizeof(RAWINPUTHEADER)) != dwSize )
						 OutputDebugString (TEXT("GetRawInputData does not return correct size !\n")); 

					RAWINPUT* raw = (RAWINPUT*)lpb;

					if (raw->header.dwType == RIM_TYPEKEYBOARD) 
					{
#if _CAPTUREKEYBOARD
#if _DEBUGRAW
						char s[32];
						
						GetKeyNameTextA(raw->data.keyboard.VKey, s, 32);
						printf("[WndProc - WM_INPUT - Keyboard]\t%i, %i, %i, %i, %i, %i\t%s%i\n",	
							raw->data.keyboard.MakeCode, 
							raw->data.keyboard.Flags, 
							raw->data.keyboard.Reserved, 
							raw->data.keyboard.ExtraInformation, 
							raw->data.keyboard.Message, 
							raw->data.keyboard.VKey, s, GetLastError());
#endif
						pKeyHandler->handleRawKey(raw->data.keyboard.MakeCode, raw->data.keyboard.Flags);
#endif
					}
					else if (raw->header.dwType == RIM_TYPEMOUSE) 
					{
#if _CAPTUREMOUSE
#if _DEBUGRAW
						printf("[WndProc - WM_INPUT - Mouse]\t%i, %i, %i, %i, %i, %i, %i, %i\n",	
							raw->data.mouse.usFlags, 
							raw->data.mouse.ulButtons, 
							raw->data.mouse.usButtonFlags, 
							raw->data.mouse.usButtonData, 
							raw->data.mouse.ulRawButtons, 
							raw->data.mouse.lLastX, 
							raw->data.mouse.lLastY, 
							raw->data.mouse.ulExtraInformation);
#endif
						LONG mouseX = raw->data.mouse.lLastX;
						LONG mouseY = raw->data.mouse.lLastY;
						if(mouseX != 0 || mouseY != 0)
						{
							pMouseHandler->mouseMove(mouseX, mouseY);
						}else
						{
							
							pMouseHandler->mouseButtonEvent(raw->data.mouse.ulButtons, raw->data.mouse.usButtonData);
						}
#endif
					} 
					delete[] lpb; 
					return 0;
				} 
			}
			return DefWindowProc( hwnd, message, wparam, lparam );
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

			return 0;
		}
		void setStyleBorderless(void)
		{
			LONG lStyle = GetWindowLong(g->hwnd, GWL_STYLE);
			lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
			lStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
			SetWindowLong(g->hwnd, GWL_STYLE, lStyle);
		}
		void getScreenResolution(RECT* result)
		{
		   const HWND hDesktop = GetDesktopWindow();
		   GetWindowRect(hDesktop, result);
		}




		
		static GLuint fallbackTextureID;
		static GL::Model modelToRender;
		static bool doOnceAtStart = false;
		RenderManager::RenderManager(void)
		{
			//pGameCamera->setPos(0, 0, 10);
			//pGameCamera->setPitch(95);
			//pGameCamera->setYaw(275);
		}
		RenderManager::~RenderManager(void)
		{
		}
		
		//void RenderManager::initOpenGL(void)
		//{
		//	//TODO
		//	//Check http://www.opengl.org/wiki/Tutorial1:_Rendering_shapes_with_glDrawRangeElements,_VAO,_VBO,_shaders_%28C%2B%2B_/_freeGLUT%29 for initialisation (to see how it should be instead of how it is currently)
		//	glViewport(0, 0, g->renderWidth, g->renderHeight);
		//	glMatrixMode(GL_PROJECTION);
		//	glEnable(GL_DEPTH_TEST);
		//	glDepthFunc(GL_LEQUAL);
		//	gluPerspective(45.0,(float)g->renderWidth/(float)g->renderHeight, 1, 1000);
		//	glEnable(GL_TEXTURE_2D);
		//	glMatrixMode(GL_MODELVIEW);
		//	glLoadIdentity();
		//	glShadeModel(GL_SMOOTH);
		//	//glEnable(GL_TEXTURE_COORD_ARRAY);
		//	//glEnable(GL_CULL_FACE);
		//	//glFrontFace(GL_CCW);
		//	//glCullFace(GL_BACK);
		//	CheckForOpenGLErrors();
		//}

		void RenderManager::draw(void)
		{
			////wireframe:
			//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			////normal:
			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

			static GLuint fallbackTextureID;
			
			glClearColor( 0, 0, 0, 0 );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glViewport(0, 0, g->renderWidth, g->renderHeight);
			gluPerspective(45.0,(float)g->renderWidth/(float)g->renderHeight, 1, 1000);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);

			pGameCamera->invokeGluLookAt();
			CheckForOpenGLErrors();

			if(!doOnceAtStart)
			{
				doOnceAtStart = true;
				fallbackTextureID = load_texture_TGA("H:\\Visual Studio 2010\\OpenGL_Test\\src\\Debug\\materials\\default\\fallback.tga", NULL, NULL, 0, 0);
				//readModelFile("C:\\Users\\Marco.Silipo\\Desktop\\untitled.obj", modelToRender);
				modelToRender = GL::Model();
				CheckForOpenGLErrors();
				modelToRender.readObjFile("C:\\Users\\Marco.Silipo\\Desktop\\untitled.obj");
				CheckForOpenGLErrors();
				modelToRender.loadModelIntoGPU();
				CheckForOpenGLErrors();
				modelToRender.readVertexShader("H:\\Visual Studio 2010\\OpenGL_Test\\src\\Debug\\shaders\\default.vs");
				CheckForOpenGLErrors();
				modelToRender.readFragmentShader("H:\\Visual Studio 2010\\OpenGL_Test\\src\\Debug\\shaders\\default.fs");
				CheckForOpenGLErrors();
				modelToRender.combineShaders();
				CheckForOpenGLErrors();
			}
			glTranslated(0, -1, 0);
			glColor3f(0, 1, 0);
			modelToRender.render();
			glTranslated(0, -3, 0);
			glBegin(GL_TRIANGLES);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(50.0f, -2.0f, 50.0f);
				glTexCoord2f(0.0f, 50.0f); glVertex3f(50.0f, -2.0f, -50.0);
				glTexCoord2f(50.0f, 0.0f); glVertex3f(-50.0f, -2.0f, 50.0f);
			
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-50.0f, -2.0f, -50.0f);
				glTexCoord2f(0.0f, 50.0f); glVertex3f(50.0f, -2.0f, -50.0);
				glTexCoord2f(50.0f, 0.0f); glVertex3f(-50.0f, -2.0f, 50.0f);
			glEnd();

			SwapBuffers(g->hdc);
			CheckForOpenGLErrors();
		}

		
	bool RenderManager::CheckForOpenGLErrors(void)
	{
		int i;
		bool flag = false;
		std::string errorString = std::string();
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
			std::printf("[GLError] got following errors: \n%s", errorString.c_str());
		}
		return flag;
	}
		
	};
};