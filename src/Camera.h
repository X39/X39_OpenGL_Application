#pragma once

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


namespace X39
{
	namespace Singletons
	{
		class Camera
		{

			private: 
				double pitch;
				double yaw;
				double roll;
				::glm::vec3 pos;
				glm::mat4 viewPort;

				Camera(void);
				Camera(Camera const&);

				void operator=(Camera const&);

			public:
				~Camera(void);
			
				double getPitch(void);
				double getYaw(void);
				double getRoll(void);
				::glm::vec3 getPos(void);
			
				void setPitch(double val);
				void setYaw(double val);
				void setRoll(double val);
				void setPos(::glm::vec3 val);
				::glm::vec3 addPos(::glm::vec3 val);
			
				glm::mat4 recalculateViewPort(void);
				glm::mat4 getViewPort(void);
				static Camera& getInstance(void);
		};
	}
}