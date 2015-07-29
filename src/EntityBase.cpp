#include "EntityBase.h"
#include "Simulation.h"
::X39::Entity::EntityBase::EntityBase()
{
	::X39::Simulation::getInstance().addEntity(this);
}
::X39::Entity::EntityBase::~EntityBase()
{
	::X39::Simulation::getInstance().removeEntity(this);
}
void ::X39::Entity::EntityBase::kill()
{
	this->onKill();
	::X39::Simulation::getInstance().removeEntity(this);
}