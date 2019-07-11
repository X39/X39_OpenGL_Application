#include "TestWorld.hpp"
#include "MainMenu.hpp"
#include "Simulation.h"
#include "GlobalObject.h"
#include "DisplayBase.h"

namespace X39
{
	void TestWorld::render()
	{
	}
	void TestWorld::update()
	{
	}
	void TestWorld::prepareWorld()
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				for (int k = 0; k < 10; k++)
			entities.push_back(new X39::Entity::NormalizedEntity(i, j, k));

		auto ne = new X39::Entity::NormalizedEntity(0);
		entities.push_back(ne);
		ne->setEnableMovement(false);
		ne->setPosition(0, 0, -2);

		testButton = new ::X39::GUI::DCButton(
			0,
			::X39::GlobalObject::getInstance().render_height - 400,
			160,
			20,
			[] { ::X39::Simulation::getInstance().setWorld(new MainMenu()); },
			std::string("Main Menu"),
			*::X39::Singletons::MaterialManager::getInstance().getMaterialByVmatPath("Materials\\ui_base\\ui_base.vmat"),
			0
			);
		::X39::GlobalObject::getInstance().mainDisplay->addChild(testButton);
	}
	void TestWorld::destroyWorld()
	{
		::X39::GlobalObject::getInstance().mainDisplay->clearControls();
		delete testButton;
		for (auto& it : entities)
			::X39::Simulation::getInstance().removeEntity(it);
		entities.clear();
	}
}