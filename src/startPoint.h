#pragma once
#include "globals.h"
#include "guiBase.h"


#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>
#include <omp.h>


int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );
void calculateExecutivePath(void);