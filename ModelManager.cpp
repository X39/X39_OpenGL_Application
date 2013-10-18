#include "ModelManager.h"


namespace X39
{
	namespace Rendering
	{
		ModelManager::ModelManager(void)
		{
		}
		ModelManager::~ModelManager(void)
		{
		}

		void ModelManager::registerModels(void)
		{
			/*
			WIN32_FIND_DATA ffd;
			TCHAR* szDir = "";
			HANDLE hFind = INVALID_HANDLE_VALUE;


			hFind = FindFirstFile(szDir, &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
				return;
			std::string exePath = std::string(g->executivePath);
			do
			{
				std::string filePath = std::string(ffd.cFileName);
				if(filePath.rfind(".x39") == std::string::npos)
					continue;
				structModel* model = new structModel;
				readModelFile(filePath, model);

				delete model;
			}
			while (FindNextFile(hFind, &ffd) != 0);
			FindClose(hFind);
			*/
		}

//		void ModelManager::bindModelToGraCaBuffer(structModel* model)
//		{
//			/*
//			http://www.gamedev.net/topic/422214-attractors---big-number-of-points-problem/?view=findpost&p=3811813
//			glEnableClientState(GL_VERTEX_ARRAY) - if we want to specify an array containing vertex data
//
//			glEnableClientState(GL_TEXTURE_COORD_ARRAY)- if we want to specify an array containing texture coords data(for each vertex)
//
//			glEnableClientState(GL_NORMAL_ARRAY)- if we want to specify an array containing normal data(for each vertex)
//
//			glEnableClientState(GL_COLOR_ARRAY)- if we want to specify an array containing color data(for each vertex)
//			*/
//			/*
//			glEnableClientState(GL_VERTEX_ARRAY);
//			if(model == NULL)
//				return;
//			GLsizei buffersToGen = 1;
//			GLuint tempBuffer[2] = {0, 0};
//			glGenBuffers(buffersToGen, tempBuffer);
//			
//			model->indexBufferID = tempBuffer[0];
//			glBindBuffer(GL_ARRAY_BUFFER, model->indexBufferID);
//			glVertexPointer(3, GL_FLOAT, 0, NULL);
//			glBufferData(GL_ARRAY_BUFFER, structModel_GetVectorList(model)->size() * sizeof(GLfloat), structModel_GetVectorList(model)->data(), (model->isStaticDraw > 1 ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW));
//			*/
//		}
//		void ModelManager::unbindModelFromGraCaBuffer(structModel* model)
//		{
//			/*
//			if(model == NULL)
//				return;
//			GLsizei buffersToDel = 1;
//			glDeleteBuffers(buffersToDel, &model->indexBufferID);
//			model->indexBufferID = 0;
//			*/
//		}
	};
};