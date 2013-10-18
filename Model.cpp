#include "Model.h"

namespace X39
{
	namespace Rendering
	{
		namespace GL
		{
			Model::Model(void)
			{
				verticesList = std::vector<GLfloat>();
				normalsList = std::vector<GLfloat>();
				textureCoordList = std::vector<GLfloat>();
				indexList = std::vector<GLushort>();
				glVerticesBufferID = 0;
				glIndexBufferID = 0;
				isLoadedInGPU = false;
				isModelReadFromFile = false;
				glVertexShader = -1;
				glFragmentShader = -1;
				glShaderProgram = -1;
			}
			Model::~Model(void)
			{
				if(isLoadedInGPU)
				{
					unloadModelFromGPU();
				}
				verticesList.clear();
				normalsList.clear();
				textureCoordList.clear();
				indexList.clear();
			}

			bool Model::readObjFile(const char* pathCStr)
			{
				//prevent loading model data twice
				if(this->isModelReadFromFile)
					return false;

				//init some stuff
				this->isModelReadFromFile = true;
				std::ifstream fIn (pathCStr, std::ifstream::in);
				if(!fIn)
				{
					return false;
				}

				//Parse .obj file
				char line[256];
				while(!fIn.eof())
				{
					fIn.getline(line, 256);
					if(line[0] == 'v')
					{
						if(line[1] == ' ')//vertices
						{
							char* s = std::strstr(line, " ");
							this->verticesList.push_back((float)std::atof(s + 1));
							s = std::strstr(s + 1, " ");
							this->verticesList.push_back((float)std::atof(s + 1));
							s = std::strstr(s + 1, " ");
							this->verticesList.push_back((float)std::atof(s + 1));
						}else if(line[1] == 't')//texture coords
						{
							char* s = std::strstr(line, " ");
							this->textureCoordList.push_back((float)std::atof(s + 1));
							s = std::strstr(s + 1, " ");
							this->textureCoordList.push_back((float)std::atof(s + 1));
						}else if(line[1] == 'n')//normals
						{
							char* s = std::strstr(line, " ");
							this->normalsList.push_back((float)std::atof(s + 1));
							s = std::strstr(s + 1, " ");
							this->normalsList.push_back((float)std::atof(s + 1));
							s = std::strstr(s + 1, " ");
							this->normalsList.push_back((float)std::atof(s + 1));
						}

					} else if(line[0] == 'f')
					{
						char* s = std::strstr(line, " ");
						GLushort tempUShort[9];
						tempUShort[0] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[3] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[6] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, " ");			
																
						tempUShort[1] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[4] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[7] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, " ");			
																
						tempUShort[2] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[5] = (GLushort)std::atoi(s + 1) - 1;
						s = std::strstr(s + 1, "/");			
						tempUShort[8] = (GLushort)std::atoi(s + 1) - 1;

						for(int i = 0; i < 9; i++)
							this->indexList.push_back(tempUShort[i]);
					}
				}
				return true;
			}
			bool Model::readVertexShader(const char* pathCStr)
			{
				/* source: http://open.gl/drawing
				 *	Checking if a shader compiled successfully 
				 *
				 *	GLint status;
				 *	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status); 
				 *
				 *	If status is equal to GL_TRUE, then your shader was compiled successfully. 
				 *
				 *	Retrieving the compile log 
				 *
				 *	char buffer[512];
				 *	glGetShaderInfoLog(vertexShader, 512, NULL, buffer); 
				 *
				 *	This will store the first 511 bytes + null terminator of the compile log in the specified buffer.
				 *	The log may also report useful warnings even when compiling was successful,
				 *	so it's useful to check it out from time to time when you develop your shaders.
				 */
				if(glVertexShader == -1)
					return false;
				glVertexShader = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(glVertexShader, 1, &pathCStr, NULL);
				glCompileShader(glVertexShader);
				return true;
			}
			bool Model::readFragmentShader(const char* pathCStr)
			{
				/* source: http://open.gl/drawing
				 *	Checking if a shader compiled successfully 
				 *
				 *	GLint status;
				 *	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status); 
				 *
				 *	If status is equal to GL_TRUE, then your shader was compiled successfully. 
				 *
				 *	Retrieving the compile log 
				 *
				 *	char buffer[512];
				 *	glGetShaderInfoLog(vertexShader, 512, NULL, buffer); 
				 *
				 *	This will store the first 511 bytes + null terminator of the compile log in the specified buffer.
				 *	The log may also report useful warnings even when compiling was successful,
				 *	so it's useful to check it out from time to time when you develop your shaders.
				 */
				if(glFragmentShader == -1)
					return false;
				glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(glFragmentShader, 1, &pathCStr, NULL);
				glCompileShader(glFragmentShader);
				return true;
			}
			void Model::combineShaders(void)
			{
				glShaderProgram = glCreateProgram();
				glAttachShader(glShaderProgram, glVertexShader);
				glAttachShader(glShaderProgram, glFragmentShader);
				glBindFragDataLocationEXT(glShaderProgram, 0, "outColor");
				glLinkProgram(glShaderProgram);
			}
			void Model::loadModelIntoGPU(void)
			{
				glGenBuffers(1, &this->glVerticesBufferID);
				glBindBuffer(GL_ARRAY_BUFFER, this->glVerticesBufferID);
				glBufferData(GL_ARRAY_BUFFER, (this->verticesList.size() + this->textureCoordList.size() + this->normalsList.size()) * sizeof(GLfloat), 0, GL_STATIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER,
								0,
								this->verticesList.size() * sizeof(GLfloat),
								this->verticesList.data()
							   );
				glBufferSubData(
								GL_ARRAY_BUFFER, 
								this->verticesList.size() * sizeof(GLfloat), 
								this->normalsList.size() * sizeof(GLfloat), 
								this->normalsList.data()
							   );
				glBufferSubData(
								GL_ARRAY_BUFFER, 
								(this->verticesList.size() + this->normalsList.size()) * sizeof(GLfloat), 
								this->textureCoordList.size() * sizeof(GLfloat),
								this->textureCoordList.data()
							   );

				glGenBuffers(1, &this->glIndexBufferID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glIndexBufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indexList.size() * sizeof(GLushort), this->indexList.data(), GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);



				pub_glVerticesBufferID = glVerticesBufferID;
				pub_glIndexBufferID = glIndexBufferID;
			}
			void Model::unloadModelFromGPU(void)
			{
				this->isLoadedInGPU = true;
			}
			void Model::render(void)
			{
				glUseProgram(glShaderProgram);
				glBindBuffer(GL_ARRAY_BUFFER, this->glVerticesBufferID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->glIndexBufferID);
				glVertexPointer(3, GL_FLOAT, 3 * sizeof(GLfloat), (GLvoid*)0);
				glNormalPointer(GL_FLOAT, 3 * sizeof(GLfloat), (GLvoid*)(this->verticesList.size() * sizeof(GLfloat)));
				glTexCoordPointer(2, GL_FLOAT, 2 * sizeof(GLfloat), (GLvoid*)((this->normalsList.size() + this->verticesList.size()) * sizeof(GLfloat)));

				glDrawElements(GL_TRIANGLES, this->indexList.size() / 9 * 3, GL_UNSIGNED_SHORT, 0);
				//glDrawArrays(GL_TRIANGLES, 0, this->indexList.size() / 9);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		};
	};
};