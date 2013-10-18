#include "Position.h"


namespace X39
{
	namespace EngineObjects
	{
		Position::Position(void)
		{
			posX = 0;
			posY = 0;
			posZ = 0;
			smallPosX = 0.0F;
			smallPosY = 0.0F;
			smallPosZ = 0.0F;
		}

		Position::~Position(void)
		{
		}


		int Position::getPosX(){return posX;}
		int Position::getPosY(){return posY;}
		int Position::getPosZ(){return posZ;}
		float Position::getSmallPosX(){return smallPosX;}
		float Position::getSmallPosY(){return smallPosY;}
		float Position::getSmallPosZ(){return smallPosZ;}

		void Position::setPosX(int val){posX = val;}
		void Position::setPosY(int val){posY = val;}
		void Position::setPosZ(int val){posZ = val;}
		void Position::setSmallPosX(float val){smallPosX = val;}
		void Position::setSmallPosY(float val){smallPosY = val;}
		void Position::setSmallPosZ(float val){smallPosZ = val;}
	};
};
