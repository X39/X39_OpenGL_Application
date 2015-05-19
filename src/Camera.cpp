#include "globals.h"

#include <math.h>
#include <glm\gtx\euler_angles.hpp>

#include "Camera.h"

namespace X39
{
	namespace Singletons
	{
		Camera::Camera(void)
		{
			pitch = 180.0;
			yaw = 0.0;
			roll = 0.0;
			pos = ::glm::vec3();
		}

		Camera::~Camera(void)
		{
		}

	
		double Camera::getPitch(void)			{ return pitch; };
		double Camera::getYaw(void)				{ return yaw; };
		double Camera::getRoll(void)			{ return roll; };
		::glm::vec3 Camera::getPos(void)		{ return pos; };
	
		void Camera::setPitch(double val)		{ pitch = val; };
		void Camera::setYaw(double val)			{ yaw = val; };
		void Camera::setRoll(double val)		{ roll = val; };
		void Camera::setPos(::glm::vec3 val)	{ pos = val; };
		::glm::vec3 Camera::addPos(::glm::vec3 val)	{ pos += val; return pos; };
		glm::mat4 Camera::recalculateViewPort(void)
		{
			double rYaw = yaw / 180 * PIconst;
			double rPitch = pitch / 180 * PIconst;
			double rRoll = roll / 180 * PIconst;

			this->viewPort = ::glm::eulerAngleYXZ(rYaw, rPitch, rRoll);

			return this->viewPort;
		}
		glm::mat4 Camera::getViewPort(void)
		{
			return this->viewPort;
		}
		Camera& Camera::getInstance(void)
		{
			static Camera instance;
			return instance;
		}
	}
}