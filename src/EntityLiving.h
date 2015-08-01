#pragma once
#include "EntityBase.h"

namespace X39
{
	namespace Entity
	{
		struct position3D
		{
			long double x;
			long double y;
			long double z;
		};
		class EntityLiving : public EntityBase
		{
		protected:
			position3D _position;
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
				this->_position.x = posX;
				this->_position.y = posY;
				this->_position.z = posZ;
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
					this->_position.x += this->_moveX;
					this->_position.y += this->_moveY;
					this->_position.z += this->_moveZ;
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
			virtual position3D& getPosition()
			{
				return this->_position;
			}
			virtual void setPosition(const position3D& p)
			{
				this->_position.x = p.x;
				this->_position.y = p.y;
				this->_position.z = p.z;
			}
			virtual void setPosition(double x, double y, double z)
			{
				this->_position.x = x;
				this->_position.y = y;
				this->_position.z = z;
			}


			virtual void onUpdate() {};
		};
	}
}