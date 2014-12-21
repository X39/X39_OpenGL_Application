#include "GameCamera.h"

namespace X39
{
	namespace Singletons
	{
		GameCamera::GameCamera(void)
		{
			pitch = 0;
			yaw = 0.0;
			roll = 0.0;
			pos = ::glm::vec3();
		}

		GameCamera::~GameCamera(void)
		{
		}

	
		double GameCamera::getPitch(void)		{	return pitch;	};
		double GameCamera::getYaw(void)			{	return yaw;		};
		double GameCamera::getRoll(void)		{	return roll;	};
		::glm::vec3 GameCamera::getPos(void)		{	return pos;		};
	
		void GameCamera::setPitch(double val)	{	if(val > 0 && val < 180) pitch = val;	};
		void GameCamera::setYaw(double val)		{	yaw	= val;								};
		void GameCamera::setRoll(double val)	{	roll = val;								};
		void GameCamera::setPos(::glm::vec3 val){	pos = val;								};

		void GameCamera::invokeGluLookAt(void)
		{
			::glm::vec3 vec = getViewVec();
			double rRoll = roll / 180 * PIconst;
			//glTranslatef(pos.x, pos.y, pos.z);

			//gluLookAt(  pos.x, pos.y, pos.z,
			//			vec.x + pos.x,
			//			vec.y + pos.y,
			//			vec.z + pos.z,
			//			sin(rRoll),
			//			cos(rRoll),
			//			sin(rRoll)
			//			);
			double l = vec.length();
			vec.x /= l;
			vec.y /= l;
			vec.z /= l;
			vec.x += pos.x;
			vec.y += pos.y;
			vec.z += pos.z;
			gluLookAt(  pos.x, pos.y, pos.z,
						vec.x,
						vec.y,
						vec.z,
						sin(rRoll),
						cos(rRoll),
						sin(rRoll)
					 );
		}
		::glm::vec3 GameCamera::getViewVec(void)
		{
			double rPitch = pitch / 180 * PIconst;
			double rYaw = yaw / 180 * PIconst;
			::glm::vec3 vec = ::glm::vec3(cos(rYaw) * sin(rPitch), cos(rPitch), sin(rYaw) * sin(rPitch));
			//double l = vec.length();
			//vec.x /= l;
			//vec.y /= l;
			//vec.z /= l;
			return vec;
		}
		GameCamera& GameCamera::getInstance(void)
		{
			static GameCamera instance;
			return instance;
		}
	}
}