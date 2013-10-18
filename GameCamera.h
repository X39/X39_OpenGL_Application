#pragma once

#include "globals.h"
#include "glHeaders.h"

#include <math.h>


namespace X39
{
	namespace EngineObjects
	{
		class GameCamera
		{

			private: 
				double pitch;
				double yaw;
				double roll;
				glm::vec3 pos;

			public:
				GameCamera(void);
				~GameCamera(void);
				
				double getPitch(void);
				double getYaw(void);
				double getRoll(void);
				glm::vec3 getPos(void);
				
				void setPitch(double val);
				void setYaw(double val);
				void setRoll(double val);
				void setPos(glm::vec3 val);
				
				void invokeGluLookAt(void);
				glm::vec3 getViewVec();
		};
		//GameCamera* pGameCamera;
	};
};