#include "GameCamera.h"

namespace X39
{
	namespace EngineObjects
	{
		GameCamera::GameCamera(void)
		{
			pitch = 0;
			yaw = 0.0;
			roll = 0.0;
			pos = glm::vec3();
			
		}

		GameCamera::~GameCamera(void)
		{
		}

		
		double GameCamera::getPitch(void)		{	return pitch;	};
		double GameCamera::getYaw(void)			{	return yaw;		};
		double GameCamera::getRoll(void)		{	return roll;	};
		glm::vec3 GameCamera::getPos(void)		{	return pos;		};
		
		void GameCamera::setPitch(double val)	{	if(val > 0 && val < 180) pitch = val;	};
		void GameCamera::setYaw(double val)		{	yaw	= val;								};
		void GameCamera::setRoll(double val)	{	roll = val;								};
		void GameCamera::setPos(glm::vec3 val)	{	pos = val;								};


		void GameCamera::invokeGluLookAt(void)
		{
			glm::vec3 vec = getViewVec();
			double rRoll = roll / 180 * PIconst;
			
			gluLookAt(  pos.x, pos.y, pos.z,
						vec.x + pos.x,
						vec.y + pos.y,
						vec.z + pos.z,
						sin(rRoll),
						cos(rRoll),
						sin(rRoll)
						);
		}
		glm::vec3 GameCamera::getViewVec()
		{
			double rPitch = pitch / 180 * PIconst;
			double rYaw = yaw / 180 * PIconst;
			return glm::vec3(cos(rYaw) * sin(rPitch), cos(rPitch), sin(rYaw) * sin(rPitch));
		}
	};
};