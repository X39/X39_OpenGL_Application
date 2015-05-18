#include "globals.h"

#include <math.h>
#include <glm\gtx\euler_angles.hpp>

#include "GameCamera.h"

namespace X39
{
	namespace Singletons
	{
		GameCamera::GameCamera(void)
		{
			pitch = 180.0;
			yaw = 0.0;
			roll = 0.0;
			pos = ::glm::vec3();
		}

		GameCamera::~GameCamera(void)
		{
		}

	
		double GameCamera::getPitch(void)			{ return pitch; };
		double GameCamera::getYaw(void)				{ return yaw; };
		double GameCamera::getRoll(void)			{ return roll; };
		::glm::vec3 GameCamera::getPos(void)		{ return pos; };
	
		void GameCamera::setPitch(double val)		{ pitch = val; };
		void GameCamera::setYaw(double val)			{ yaw = val; };
		void GameCamera::setRoll(double val)		{ roll = val; };
		void GameCamera::setPos(::glm::vec3 val)	{ pos = val; };
		::glm::vec3 GameCamera::addPos(::glm::vec3 val)	{ pos += val; return pos; };
		glm::mat4 GameCamera::recalculateViewPort(void)
		{
			double rYaw = yaw / 180 * PIconst;
			double rPitch = pitch / 180 * PIconst;
			double rRoll = roll / 180 * PIconst;

			this->viewPort = ::glm::eulerAngleYXZ(rYaw, rPitch, rRoll);

			return this->viewPort;
		}
		glm::mat4 GameCamera::getViewPort(void)
		{
			return this->viewPort;
		}
		GameCamera& GameCamera::getInstance(void)
		{
			static GameCamera instance;
			return instance;
		}
	}
}