#pragma once
#include "globals.h"
#include "EntityLiving.h"
#include "RenderBase.h"
#include <array>
#include <random>
namespace X39
{
	namespace Entity
	{
		class NormalizedEntity : public EntityLiving, public RenderBase
		{
		protected:
			GLuint test_vaoID;
			GLuint test_vertBufferID;
			GLuint test_indexBufferID;
			::X39::Shader* test_shad;
			~NormalizedEntity()
			{
				this->deconstructRenderer();
			}
		public:
			NormalizedEntity(double i) : EntityLiving(0, 1, i, 0.001 * i), RenderBase() {}
			virtual void deconstructEntity() { delete this; }
			virtual void render()
			{
				glm::mat4 viewMatrix = X39::Singletons::Camera::getInstance().getViewPort();
				glm::mat4 projectionMatrix = glm::perspective((float)45.0, (float)::X39::GlobalObject::getInstance().render_width / (float)::X39::GlobalObject::getInstance().render_height, 1.0f, 1000.0f);
				test_shad->use();
				//glTranslated(i - X39::Singletons::Camera::getInstance().getPos().x, 0 - X39::Singletons::Camera::getInstance().getPos().y, j - X39::Singletons::Camera::getInstance().getPos().z);
				glBindVertexArray(test_vaoID);
				test_shad->setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, &viewMatrix[0][0], -1);
				test_shad->setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
				test_shad->setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, &glm::mat4()[0][0], 0);
				test_shad->setUniform3fv("worldPosition", 1, &glm::vec3(
					this->_position.x - X39::Singletons::Camera::getInstance().getPos().x,
					this->_position.y - X39::Singletons::Camera::getInstance().getPos().y,
					this->_position.z - X39::Singletons::Camera::getInstance().getPos().z
					)[0], 0);
				//shad.setUniform4fv("color", 1, &glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)[0], -1);
				test_shad->setUniform1i("textureSampler", ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit, 0);

				//shad.setUniform1iv("textureSampler", 1, (GLint*)&(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit), 0);
				CheckForOpenGLErrors();
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
				test_shad->unuse();
			}
			virtual void loadRenderer()
			{
				glGenVertexArrays(1, &test_vaoID);
				glBindVertexArray(test_vaoID);

				glGenBuffers(1, &test_vertBufferID);
				glGenBuffers(1, &test_indexBufferID);

				struct Vec2 { float x, y; };
				struct Vec3 { float x, y, z; };
				struct Vert { Vec3 pos; Vec2 tex; };

				std::array<Vert, 8> cubeVerts = { {
					{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f } },
					{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f } },
					{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 1.0f } },
					{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
					{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f } },
					{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f } },
					{ { -0.5f, 0.5f, 0.5f }, { 1.0f, 1.0f } },
					{ { -0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f } }
					} };
				std::array<unsigned int, 36> cubeIdxs = { {
						1, 2, 0,
						0, 2, 3,
						4, 6, 5,
						7, 6, 4,
						1, 4, 5,
						0, 4, 1,
						6, 7, 2,
						2, 7, 3,
						7, 4, 3,
						3, 4, 0,
						6, 2, 5,
						5, 2, 1
					} };

				// Vertex buffer
				glBindBuffer(GL_ARRAY_BUFFER, test_vertBufferID);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vert) * cubeVerts.size(), cubeVerts.data(), GL_STATIC_DRAW);
				glEnableVertexAttribArray(0); // Matches layout (location = 0)
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), 0);
				glEnableVertexAttribArray(1); // Matches layout (location = 1)
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert), (GLvoid*)sizeof(Vec3));

				// Index buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, test_indexBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * cubeIdxs.size(), cubeIdxs.data(), GL_STATIC_DRAW);
				glBindVertexArray(0);
				test_shad = ::X39::Shader::getShader("Shaders\\base.shad");
			}
			virtual void unloadRenderer()
			{
				glDeleteBuffers(1, &test_vertBufferID);
				glDeleteBuffers(1, &test_indexBufferID);
				glDeleteVertexArrays(1, &test_vaoID);
			}
			virtual bool isVisible()
			{
				return true;
			}
			virtual void onUpdate()
			{
				if (this->_position.x > this->_position.z * 2 || this->_position.x < -(this->_position.z * 2))
				{
					this->_moveX *= -1;
				}
			}
		};
	}
}