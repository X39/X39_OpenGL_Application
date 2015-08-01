#pragma once
#include "WorldBase.h"
#include "DisplayBase.h"
#include "DCButton.h"
#include "GlobalObject.h"
#include "NormalizedEntity.h"
#include "Simulation.h"
#include "MainMenu.h"
#include <vector>
namespace X39
{
	class TestWorld : public WorldBase
	{
	private:
		std::vector<X39::Entity::NormalizedEntity*> entities;
		::X39::GUI::DCButton* testButton;
	public:
		virtual void render() {}
		virtual void update() {}
		virtual void prepareWorld()
		{
			for (int i = 0; i < 100; i++)
				entities.push_back(new X39::Entity::NormalizedEntity(i));

			auto ne = new X39::Entity::NormalizedEntity(0);
			entities.push_back(ne);
			ne->setEnableMovement(false);
			ne->setPosition(0, 0, -2);

			testButton = new ::X39::GUI::DCButton(
				0,
				::X39::GlobalObject::getInstance().render_height - 400,
				160,
				20,
				[] { ::X39::Simulation::getInstance().setWorld(new TestWorld()); },
				std::string("Title"),
				*::X39::Singletons::MaterialManager::getInstance().getMaterialByVmatPath("Materials\\ui_base\\ui_base.vmat"),
				0
				);
			::X39::GlobalObject::getInstance().mainDisplay->addChild(testButton);
		}
		virtual void destroyWorld()
		{
			::X39::GlobalObject::getInstance().mainDisplay->clearControls();
			delete testButton;
			for (auto& it : entities)
				::X39::Simulation::getInstance().removeEntity(it);
			entities.clear();
		}
	};
}