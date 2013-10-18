#include "EntityPlayer.h"


namespace X39
{
	namespace EntitySystem
	{
		EntityPlayer::EntityPlayer(void) : EntityBase()
		{
		}

		EntityPlayer::~EntityPlayer(void)
		{
		}
		void EntityPlayer::updateTick(void)
		{
			float pitch = pGameCamera->getPitch();
			float yaw = pGameCamera->getYaw();
			float modificator = 1.0;
			//std::printf("pitch %lf yaw %lf\n", pitch, yaw);
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_W))
			{
				float rPitch = pitch / 180 * PIconst;
				float rYaw = yaw / 180 * PIconst;
				this->entityPosition.x += (cos(rYaw) * sin(rPitch)) * modificator;
				this->entityPosition.y += (cos(rPitch)) * modificator;
				this->entityPosition.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_A))
			{
				float rPitch = 90.0 / 180 * PIconst;
				float rYaw = (yaw - 90) / 180 * PIconst;
				this->entityPosition.x += (cos(rYaw) * sin(rPitch)) * modificator;
				this->entityPosition.y += (cos(rPitch)) * modificator;
				this->entityPosition.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_S))
			{
				float rPitch = pitch / 180 * PIconst;
				float rYaw = yaw / 180 * PIconst;
				this->entityPosition.x -= (cos(rYaw) * sin(rPitch)) * modificator;
				this->entityPosition.y -= (cos(rPitch)) * modificator;
				this->entityPosition.z -= (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_D))
			{
				float rPitch = 90.0 / 180 * PIconst;
				float rYaw = (yaw + 90) / 180 * PIconst;
				this->entityPosition.x += (cos(rYaw) * sin(rPitch)) * modificator;
				this->entityPosition.y += (cos(rPitch)) * modificator;
				this->entityPosition.z += (sin(rYaw) * sin(rPitch)) * modificator;
			}
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_Spacebar))
			{
				this->entityPosition.y += 1 * modificator;
			}
			if(pKeyHandler->isKeyPressed(::EngineKeySet::KEY_LCTRL))
			{
				this->entityPosition.y -= 1 * modificator;
			}
			pGameCamera->setPos(this->entityPosition);
		}
	};
};