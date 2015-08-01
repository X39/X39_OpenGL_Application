#include "RenderBase.h"
#include "RenderManager.h"
::X39::Entity::RenderBase::RenderBase()
{
	::X39::Singletons::RenderManager::getInstance().addEntity(this);
	this->_isLoaded = false;
}
void ::X39::Entity::RenderBase::deconstructRenderer(void)
{
	::X39::Singletons::RenderManager::getInstance().removeEntity(this);
	if (this->_isLoaded)
		this->unloadRenderer();
}
void ::X39::Entity::RenderBase::doRender()
{
	if (!this->_isLoaded)
	{
		this->loadRenderer();
		this->_isLoaded = true;
	}
	this->render();
}