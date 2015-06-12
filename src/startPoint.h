#pragma once

#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
int createWindow(HINSTANCE hInstance, int iCmdShow);
void resizeWindow(int posX, int posY, int width, int height);
RECT GetDesktopResolution();
int createConsoleWindow();
int setPixelFormat();
int createRenderingContext();
int createDevices();
bool CheckForOpenGLErrors(void);
bool debugSwitchMouseMode(int mode, USHORT key);
void debugButtonTest(void);
bool doDisplayKeyHandling(int mode, USHORT key);
bool doDisplayMouseClickHandling(LPPOINT point, ULONG ulButtons, USHORT usButtonData);
bool doDisplayMouseMoveHandling(int posX, int posY, LPPOINT mousePos);
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );
void calculateExecutivePath(void);
bool CheckForOpenGLErrors(void);