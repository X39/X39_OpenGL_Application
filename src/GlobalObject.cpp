#include "globals.h"

#include "GlobalObject.h"

namespace X39
{
	GlobalObject::GlobalObject(void)
	{
		this->windowGlRenderingContextHandle = nullptr;
		this->windowHandle = nullptr;
		this->mainDisplay = nullptr;
		this->handleDeviceContext = nullptr;
	}

	GlobalObject::~GlobalObject(void)
	{
	}

	GlobalObject& GlobalObject::getInstance(void)
	{
		static GlobalObject instance;
		return instance;
	}
}