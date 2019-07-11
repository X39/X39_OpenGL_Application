#pragma once
#include "WorldBase.hpp"
#include "NormalizedEntity.h"
#include "DCButton.h"
#include <vector>
namespace X39
{
	class TestWorld : public WorldBase
	{
	private:
		std::vector<X39::Entity::NormalizedEntity*> entities;
		::X39::GUI::DCButton* testButton;
	public:
		virtual void render();
		virtual void update();
		virtual void prepareWorld();
		virtual void destroyWorld();
	};
}