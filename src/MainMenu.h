#pragma once
#include "WorldBase.h"
#include "DisplayBase.h"
#include "DCButton.h"
#include "GlobalObject.h"
namespace X39
{
	class MainMenu : public WorldBase
	{
	private:
		::X39::GUI::DCButton* testButton;
	public:
		virtual void render() {}
		virtual void update() {}
		virtual void prepareWorld()
		{
			testButton = new ::X39::GUI::DCButton(
				0,
				::X39::GlobalObject::getInstance().render_height - 400,
				160,
				20,
				[] { ::Logger::Write(Logger::INFO, "TEST"); },
				std::string("testButtonnnnnnnnnnnnnnnnnn"),
				*::X39::Singletons::MaterialManager::getInstance().getMaterialByVmatPath("Materials\\ui_base\\ui_base.vmat"),
				0
			);
			::X39::GlobalObject::getInstance().mainDisplay->addChild(testButton);
		}
		virtual void destroyWorld()
		{
			delete testButton;
		}
	};
}