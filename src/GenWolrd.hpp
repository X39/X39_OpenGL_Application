#pragma once
#include "globals.h"
#include "WorldBase.hpp"
#include <vector>
namespace X39
{
	class Chunk
	{
		double data[16][16];
		GLuint vaoID;
		GLuint vertBufferID;
		GLuint indexBufferID;
		::X39::Shader* shad;
		int posX;
		int posY;
	public:
		Chunk(int, int);
		void render(void);
		void prepare(void);
		void destroy(void);
	};
	class GenWorld : public WorldBase
	{
		std::vector<Chunk*> chunks;
	public:
		virtual void render();
		virtual void update();
		virtual void prepareWorld();
		virtual void destroyWorld();
	};
}