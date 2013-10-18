#pragma once

namespace X39
{
	namespace EngineObjects
	{
		class Position
		{

			private:
				int posX;
				int posY;
				int posZ;
				float smallPosX;
				float smallPosY;
				float smallPosZ;

			public:
				Position(void);
				~Position(void);

				int getPosX();
				int getPosY();
				int getPosZ();
				float getSmallPosX();
				float getSmallPosY();
				float getSmallPosZ();

				void setPosX(int val);
				void setPosY(int val);
				void setPosZ(int val);
				void setSmallPosX(float val);
				void setSmallPosY(float val);
				void setSmallPosZ(float val);

		};
	};
};