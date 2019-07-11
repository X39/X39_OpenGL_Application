#pragma once
namespace X39
{
	class WorldBase
	{
	public:
		virtual void render() {}
		virtual void update() {}
		virtual void prepareWorld() {}
		virtual void destroyWorld() {}
	};
}