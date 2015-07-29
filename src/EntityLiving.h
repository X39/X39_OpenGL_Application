#pragma once
#include "EntityBase.h"

namespace X39
{
	namespace Entity
	{
		class EntityLiving : public EntityBase
		{
		protected:
			double _positionX;
			double _positionY;
			double _positionZ;
			double _moveX;
			double _moveY;
			double _moveZ;
			double _gravityX;
			double _gravityY;
			double _gravityZ;

			bool _enableMovement;
		public:
			EntityLiving(double posX = 0, double posY = 0, double posZ = 0, double moveX = 0, double moveY = 0, double moveZ = 0, double gravityX = 0, double gravityY = 0, double gravityZ = 0) : EntityBase()
			{
				this->_positionX = posX;
				this->_positionY = posY;
				this->_positionZ = posZ;
				this->_moveX = moveX;
				this->_moveY = moveY;
				this->_moveZ = moveZ;
				this->_gravityX = gravityX;
				this->_gravityY = gravityY;
				this->_gravityZ = gravityZ;
				_enableMovement = true;
			}
			virtual void update()
			{
				if (_enableMovement)
				{
					this->_positionX += this->_moveX;
					this->_positionY += this->_moveY;
					this->_positionZ += this->_moveZ;
					this->_moveX -= this->_gravityX;
					this->_moveY -= this->_gravityY;
					this->_moveZ -= this->_gravityZ;
				}
				this->onUpdate();
			}
			virtual void setEnableMovement(bool flag) final
			{
				this->_enableMovement = flag;
			}
			virtual bool getEnableMovement(void) const final
			{
				return this->_enableMovement;
			}


			virtual void onUpdate() {};
		};
	}
}