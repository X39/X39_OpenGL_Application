#include "RenderManager.h"
#include "globals.h"
#include "Simulation.h"
#include "RenderBase.h"
#include <array>
#include <omp.h>


namespace X39
{
	namespace Singletons
	{
		GLuint test_vaoID;
		GLuint test_vertBufferID;
		GLuint test_indexBufferID;
		::X39::Shader test_shad;
		::X39::Simulation& simulation = ::X39::Simulation::getInstance();

		RenderManager::RenderManager()
		{
		}
		RenderManager::~RenderManager()
		{
		}


		void RenderManager::addEntity(::X39::Entity::RenderBase* ent)
		{
			this->_entityList.push_back(ent);
		}
		void RenderManager::removeEntity(::X39::Entity::RenderBase* ent)
		{
			for (int i = 0; i < this->_entityList.size(); i++)
			{
				if (this->_entityList[i] == ent)
				{
					this->_entityList[i] = this->_entityList.back();
					this->_entityList.pop_back();
					break;
				}
			}
		}
		void RenderManager::prepare(void)
		{
			glGenVertexArrays(1, &test_vaoID);
			glBindVertexArray(test_vaoID);

			glGenBuffers(1, &test_vertBufferID);
			glGenBuffers(1, &test_indexBufferID);

			struct Vec2 { float x, y; };
			struct Vec3 { float x, y, z; };
			struct Vert { Vec3 pos; Vec2 tex; };

			std::array<Vert, 8> cubeVerts = { {
				{ { 0.4f, 0.4f, 0.4f }, { 1.0f, 0.0f } },//0
				{ { 0.4f, 0.4f, -0.4f }, { 1.0f, 1.0f } },//1
				{ { 0.4f, -0.4f, -0.4f }, { 0.0f, 1.0f } },//2
				{ { 0.4f, -0.4f, 0.4f }, { 0.0f, 0.0f } },//3
				{ { -0.4f, 0.4f, 0.4f }, { 0.0f, 0.0f } },//4
				{ { -0.4f, 0.4f, -0.4f }, { 0.0f, 1.0f } },//5
				{ { -0.4f, -0.4f, -0.4f }, { 1.0f, 1.0f } },//6
				{ { -0.4f, -0.4f, 0.4f }, { 1.0f, 0.0f } }	//7
				} };

			std::array<unsigned int, 36> cubeIdxs = { {
					0, 2, 1, 0, 3, 2, // Right
					4, 5, 6, 4, 6, 7, // Left
					0, 7, 3, 0, 4, 7, // Top
					1, 6, 2, 1, 5, 6, // Bottom
					0, 5, 1, 0, 4, 5, // Front
					3, 7, 6, 3, 6, 2  // Back
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
			test_shad.load("Shaders\\base.shad");
			test_shad.compile();
		}
		void RenderManager::doRendering(void)
		{
			auto frameTime = omp_get_wtime();
			glm::mat4 viewMatrix = X39::Singletons::Camera::getInstance().recalculateViewPort();
			glm::mat4 projectionMatrix = glm::perspective((float)45.0, (float)::X39::GlobalObject::getInstance().render_width / (float)::X39::GlobalObject::getInstance().render_height, 1.0f, 1000.0f);
#pragma region CameraMovement
			::glm::vec4 vec = ::glm::vec4(0.0f);
			float pitch = (float)::X39::Singletons::Camera::getInstance().getPitch();
			float yaw = (float)::X39::Singletons::Camera::getInstance().getYaw();
			float modificator = 0.1F;
			if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LShift))
				modificator = 1;

			if (::X39::Singletons::Mouse::getInstance().getMode() == ::X39::Singletons::Mouse::MouseMode::Camera)
			{
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_W))
					vec.z -= 1 * modificator;
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_A))
					vec.x -= 1 * modificator;
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_S))
					vec.z += 1 * modificator;
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_D))
					vec.x += 1 * modificator;
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_Spacebar))
					vec.y += 1 * modificator;
				if (::X39::Singletons::KeyHandler::getInstance().isKeyPressed(::EngineKeySet::KEY_LCTRL))
					vec.y -= 1 * modificator;
			}
			vec = viewMatrix * vec;
			::X39::Singletons::Camera::getInstance().addPos(::glm::vec3(vec.x, vec.y, vec.z));
#pragma endregion
			glClearColor(0.25, 0.25, 0.25, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			glEnable(GL_BLEND);
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_LINE_SMOOTH);
			//http://wiki.delphigl.com/index.php/glBlendFunc for different examples
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

			glPushMatrix();
			::X39::Singletons::MaterialManager::getInstance().loadMaterial(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0));
			//::X39::Singletons::MaterialManager::getInstance().loadMaterial(::X39::Singletons::FontManager::getInstance().getFont(0)->material, ::X39::Singletons::FontManager::getInstance().getCharTextureIndex(::X39::Singletons::FontManager::getInstance().getFont(0), 'a'));

			test_shad.use();
			for (float posX = 5; posX > -5; posX--)
			{
				for (float posY = 5; posY > -5; posY--)
				{
					for (float posZ = 5; posZ > -5; posZ--)
					{
						if ((posX != 0 || posY != 0 || posZ != 0))
							continue;
						//glTranslated(i - X39::Singletons::Camera::getInstance().getPos().x, 0 - X39::Singletons::Camera::getInstance().getPos().y, j - X39::Singletons::Camera::getInstance().getPos().z);
						glBindVertexArray(test_vaoID);
						test_shad.setUniformMatrix4fv("viewMatrix", 1, GL_FALSE, &viewMatrix[0][0], -1);
						test_shad.setUniformMatrix4fv("projectionMatrix", 1, GL_FALSE, &projectionMatrix[0][0], -1);
						test_shad.setUniformMatrix4fv("modelMatrix", 1, GL_FALSE, &glm::mat4()[0][0], 0);
						test_shad.setUniform3fv("worldPosition", 1, &glm::vec3(
							posX - X39::Singletons::Camera::getInstance().getPos().x,
							posY - X39::Singletons::Camera::getInstance().getPos().y,
							posZ - X39::Singletons::Camera::getInstance().getPos().z
							)[0], 0);
						//shad.setUniform4fv("color", 1, &glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)[0], -1);
						test_shad.setUniform1i("textureSampler", ::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit, 0);

						//shad.setUniform1iv("textureSampler", 1, (GLint*)&(::X39::Singletons::MaterialManager::getInstance().getMaterialByIndex(0)->textures[0]->textureUnit), 0);
						CheckForOpenGLErrors();
						glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
						glBindVertexArray(0);
					}
				}
			}
			test_shad.unuse();
			glPopMatrix();
			glPushMatrix();
			for (auto& it : this->_entityList)
			{
				it->doRender();
			}
			glPopMatrix();

			//2D projection
			glPushMatrix();
			glLoadIdentity();
			glLoadIdentity();
			glDisable(GL_CULL_FACE);
			glClear(GL_DEPTH_BUFFER_BIT);
			glm::vec3 camPos = ::X39::Singletons::Camera::getInstance().getPos();
			char s[256];
			sprintf(s, "POS: %lf, %lf, %lf\nPITCH: %lf, YAW: %lf, ROLL %lf", camPos.x, camPos.y, camPos.z, ::X39::Singletons::Camera::getInstance().getPitch(), ::X39::Singletons::Camera::getInstance().getYaw(), ::X39::Singletons::Camera::getInstance().getRoll());
			::X39::GUI::GuiBase::drawText2D(::X39::Singletons::FontManager::getInstance().getFont(0), s, 1.5, 0, 0);

			//char s[256];
			frameTime = omp_get_wtime() - frameTime;
			sprintf(s, "FPS: %i", (int)(1 / frameTime));
			::X39::GUI::GuiBase::drawText2D(::X39::Singletons::FontManager::getInstance().getFont(0), s, 1.5, 0, 500);
			//::X39::GUI::GuiBase::drawText2D(::X39::Singletons::FontManager::getInstance().getFont(0), "abcdefghijklm\nnopqrstuvwxyz\nABCDEFGHIJKLM\nNOPQRSTUVWXYZ\n1234567890", 10, 0, 0);

			//::X39::GUI::GuiBase::drawChar2D(::X39::Singletons::FontManager::getInstance().getFont(0), 'A', 0, 0, 200, 200);
			//::X39::GUI::GuiBase::drawChar2D(::X39::Singletons::FontManager::getInstance().getFont(0), 'B', 200, 0, 200, 200);
			//::X39::GUI::GuiBase::drawChar2D(::X39::Singletons::FontManager::getInstance().getFont(0), 'C', 200, 200, 200, 200);
			//::X39::GUI::GuiBase::drawChar2D(::X39::Singletons::FontManager::getInstance().getFont(0), 'D', 0, 200, 200, 200);

			//::X39::Singletons::FontManager::getInstance().fontShader.use();
			//::X39::GUI::GuiBase::drawTexture2D(
			//	::X39::Singletons::FontManager::getInstance().getFont(0)->material,
			//	::X39::Singletons::FontManager::getInstance().getCharTextureIndex(::X39::Singletons::FontManager::getInstance().getFont(0), 'A'),
			//	0, 0, 16, 16,
			//	0, 0, 256, 256,
			//	::X39::Singletons::FontManager::getInstance().fontShader
			//);
			//::X39::Singletons::FontManager::getInstance().fontShader.unuse();
			::X39::GlobalObject::getInstance().mainDisplay->draw();
			glPopMatrix();

			SwapBuffers(::X39::GlobalObject::getInstance().handleDeviceContext);
			CheckForOpenGLErrors();
		}
	}
}