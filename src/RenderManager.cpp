#include "RenderManager.h"
#include "globals.h"
#include "Simulation.h"
#include "RenderBase.h"
#include "WorldBase.hpp"
#include <array>
#include <omp.h>


namespace X39
{
	namespace Singletons
	{
		::X39::Simulation& simulation = ::X39::Simulation::getInstance();
		::X39::WorldBase* currentWorld = nullptr;

		RenderManager::RenderManager()
		{
		}
		RenderManager::~RenderManager()
		{
		}


		void RenderManager::setWorld(::X39::WorldBase* world)
		{
			currentWorld = world;
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
		}
		void RenderManager::doRendering(void)
		{
			auto frameTime = omp_get_wtime();
			glm::mat4 viewMatrix = X39::Singletons::Camera::getInstance().recalculateViewPort();
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
			for (auto& it : this->_entityList)
			{
				it->doRender();
			}
			if (currentWorld)
				currentWorld->render();
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
			if (::X39::GlobalObject::getInstance().mainDisplay)
				::X39::GlobalObject::getInstance().mainDisplay->draw();
			glPopMatrix();

			SwapBuffers(::X39::GlobalObject::getInstance().handleDeviceContext);
			CheckForOpenGLErrors();
		}
	}
}