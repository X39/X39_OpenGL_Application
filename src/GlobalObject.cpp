#include "globals.h"

#include "GlobalObject.h"

namespace X39
{
	GlobalObject::GlobalObject(void)
	{
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