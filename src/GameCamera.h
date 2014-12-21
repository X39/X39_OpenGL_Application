#pragma once

#include "globals.h"


#include <math.h>


namespace X39
{
	namespace Singletons
	{
		class GameCamera
		{

			private: 
				double pitch;
				double yaw;
				double roll;
				::glm::vec3 pos;

				GameCamera(void);
				GameCamera(GameCamera const&);

				void operator=(GameCamera const&);

			public:
				~GameCamera(void);
			
				double getPitch(void);
				double getYaw(void);
				double getRoll(void);
				::glm::vec3 getPos(void);
			
				void setPitch(double val);
				void setYaw(double val);
				void setRoll(double val);
				void setPos(::glm::vec3 val);
			
				void invokeGluLookAt(void);
				::glm::vec3 getViewVec(void);
				static GameCamera& getInstance(void);
		};
	}
}