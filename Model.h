#pragma once

#include "globals.h"
#include "glHeaders.h"


#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
namespace X39
{
	namespace Rendering
	{
		namespace GL
		{
			class Model
			{
			private:
				std::vector<GLfloat> verticesList;
				std::vector<GLfloat> normalsList;
				std::vector<GLfloat> textureCoordList;
				std::vector<GLushort> indexList;
				bool isLoadedInGPU;
				bool isModelReadFromFile;
				GLuint glIndexBufferID;//GL_ELEMENT_ARRAY_BUFFER
				GLuint glVerticesBufferID;//GL_ARRAY_BUFFER
				GLuint glVertexShader;
				GLuint glFragmentShader;
				GLuint glShaderProgram;
			public:
				GLuint pub_glIndexBufferID;//not used internal (only for fast acces from outside)
				GLuint pub_glVerticesBufferID;//not used internal (only for fast acces from outside)
			public:
				Model::Model(void);
				Model::~Model(void);
				bool readObjFile(const char* pathCStr);
				bool readVertexShader(const char* pathCStr);
				bool readFragmentShader(const char* pathCStr);
				void combineShaders(void);
				void loadModelIntoGPU(void);
				void unloadModelFromGPU(void);
				void render(void);
			};
		};
	};
};