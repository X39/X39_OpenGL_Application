#pragma once
#include "WorldBase.hpp"
#include "DisplayBase.h"
#include "DCButton.h"
#include "GlobalObject.h"
#include "Simulation.h"
#include "TestWorld.hpp"
#include "GenWolrd.hpp"
namespace X39
{
	class MainMenu : public WorldBase
	{
	private:
		::X39::GUI::DCButton* testWorldButton;
		::X39::GUI::DCButton* genWorldButton;
	public:
		virtual void render() {}
		virtual void update() {}
		virtual void prepareWorld()
		{
			testWorldButton = new ::X39::GUI::DCButton(
				0,
				::X39::GlobalObject::getInstance().render_height - 400,
				160,
				20,
				[] { ::X39::Simulation::getInstance().setWorld(new TestWorld()); },
				std::string("TestWorld"),
				*::X39::Singletons::MaterialManager::getInstance().getMaterialByVmatPath("Materials\\ui_base\\ui_base.vmat"),
				0
				);
			genWorldButton = new ::X39::GUI::DCButton(
				0,
				::X39::GlobalObject::getInstance().render_height - 300,
				160,
				20,
				[] { ::X39::Simulation::getInstance().setWorld(new GenWorld()); },
				std::string("GenWorld"),
				*::X39::Singletons::MaterialManager::getInstance().getMaterialByVmatPath("Materials\\ui_base\\ui_base.vmat"),
				0
				);
			::X39::GlobalObject::getInstance().mainDisplay->addChild(testWorldButton);
			::X39::GlobalObject::getInstance().mainDisplay->addChild(genWorldButton);
		}
		virtual void destroyWorld()
		{
			::X39::GlobalObject::getInstance().mainDisplay->clearControls();
			delete testWorldButton;
			delete genWorldButton;
		}
	};
}