#include "GenWolrd.hpp"
#include <random>
#include <array>
//#include <omp.h>
namespace X39
{

	Chunk::Chunk(int posX, int posY) : posX(posX), posY(posY)
	{
		for (int i = 0; i < 16; i++)
			for (int j = 0; j < 16; j++)
				data[i][j] = (rand() % 3) / 10.0;

	}
	void Chunk::render()
	{
		glm::mat4 viewMatrix = X39::Singletons::Camera::getInstance().getViewPort();
		glm::mat4 projectionMatrix = glm::perspective((float)45.0, (float)::X39::GlobalObject::getInstance().render_width / (float)::X39::GlobalObject::getInstance().render_height, 1.0f, 1000.0f);
		shad->use();
		//glTranslated(i - X39::Singletons::Camera::getInstance().getPos().x, 0 - X39::Singletons::Camera::getInstance().getPos().y, j - X39::Singletons::Camera::getInstance().getPos().z);
		glBindVertexArray(vaoID);
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				auto spot = data[i][j];
				shad->setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, &viewMatrix[0][0], -1);
				shad->setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
				shad->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, &glm::mat4()[0][0], 0);
				shad->setUniform3fv("worldPosition", 1, &glm::vec3(
					(this->posX * 16) + i - X39::Singletons::Camera::getInstance().getPos().x,
					spot - X39::Singletons::Camera::getInstance().getPos().y,
					(this->posY * 16) + j - X39::Singletons::Camera::getInstance().getPos().z
					)[0], 0);
				//shad.setUniform4fv("color", 1, &glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)[0], -1);
				shad->setUniform1i("textureSampler", ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit, 0);

				//shad.setUniform1iv("textureSampler", 1, (GLint*)&(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit), 0);
//				CheckForOpenGLErrors();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
//		glBindVertexArray(0);
//		shad->unuse();
	}
	void Chunk::prepare()
	{
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		glGenBuffers(1, &vertBufferID);
		glGenBuffers(1, &indexBufferID);

		struct Vec2 { float x, y; };
		struct Vec3 { float x, y, z; };
		struct Vert { Vec3 pos; Vec2 tex; };

		//std::vector<Vert> cubeVerts;
		//std::vector<unsigned int> cubeIdxs;
		//
		//for (int i = 0; i < 16; i++)
		//{
		//	for (int j = 0; j < 16; j++)
		//	{
		//		auto spot = data[i][j];
		//		cubeVerts.push_back({ { i, spot, j }, { 1, 0 } });
		//	}
		//}
		//for (int i = 0; i < 15; i++)
		//{
		//	for (int j = 0; j < 15; j++)
		//	{
		//		cubeIdxs.push_back(i * 16 + j);
		//		cubeIdxs.push_back(i * 16 + j + 1);
		//		cubeIdxs.push_back((i + 1) * 16 + j + 1);
		//	}
		//}

		std::array<Vert, 8> cubeVerts = { {
			{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } }
			} };
		std::array<unsigned int, 6> cubeIdxs = { {
				1, 2, 0,
				0, 2, 3
			} };

		// Vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // Matches layout (location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
		glEnableVertexAttribArray(1); // Matches layout (location = 1)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
		glBindVertexArray(0);
		shad = ::X39::Shader::getShader("Shaders\\base.shad");
	}
	void Chunk::destroy()
	{
		glDeleteBuffers(1, &vertBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteVertexArrays(1, &vaoID);
	}



	void GenWorld::render()
	{
//		auto frameTime = omp_get_wtime();
		for (int i = 0; i < chunks.size(); i++)
		{
			chunks[i]->render();
		}
//		frameTime = omp_get_wtime() - frameTime;
//		if (frameTime > 0.1)
//			LOGGER_WRITE(Logger::DEBUG, std::to_string(frameTime));
//		LOGGER_WRITE(Logger::DEBUG, std::to_string(frameTime));
	}
	void GenWorld::update() {}
	void GenWorld::prepareWorld()
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				auto chunk = new Chunk(i, j);
				chunk->prepare();
				chunks.push_back(chunk);
			}
	}
	void GenWorld::destroyWorld()
	{
		for (auto& it : chunks)
		{
			it->destroy();
			delete it;
		}
		chunks.clear();
	}
}