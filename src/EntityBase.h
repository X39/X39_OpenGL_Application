#pragma once

namespace X39
{
	namespace Entity
	{
		class EntityBase
		{
		private:
			bool _isRenderable;
			bool _isSerializable;
		public:
			EntityBase(bool isRenderable = false, bool isSerializable = false) : _isRenderable(isRenderable), _isSerializable(isSerializable) {}
			inline bool isRenderable() const { return this->_isRenderable; }
			inline bool isSerializable() const { return this->_isSerializable; }

			virtual void update() = 0;
		};
	}
}