#include "Shader.h"
#include "globals.h"
#include <dotX39\DocumentReader.h>
#include <dotX39\DataString.h>
#include "CommandHandler\CommandHandler.h"

#include <iostream>

using namespace dotX39;
namespace X39
{
	Shader::Shader(void)
	{
	}

	Shader::~Shader(void)
	{
	}

	bool Shader::load(std::string path)
	{
		//ToDo: add attribute definition to .shad files again
		//Some old code:
		//	for(unsigned int layer1Index = 0; layer1Index < layer1.boundOptions.size(); layer1Index++)
		//	{
		//		SettingsOption opt = *(layer1.boundOptions[layer1Index]);
		//		if(strcmp(opt.getName(), "attributes") == 0)
		//		{
		//			double d = 0;
		//			if(!::CommandHandler::convAsciiCharToDouble(opt.getValue(), &d))
		//			{
		//				LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(name.getValue())).append("'.").append(" Please check '").append(opt.getName()).append("'. Cannot read vertexShader file"));
		//				fileStream.close();
		//				return false;
		//			}
		//			vertexShaderAttributeLocations.push_back((unsigned int)d);
		//		}
		//	}
		std::string dir = path.substr(0, path.find_last_of("/\\"));
		Node* root;
		LOGGER_WRITE(::Logger::INFOImportant, std::string("Loading Shader '").append(path).append("' to RAM"));
		try
		{
			root = new Node("root");
			DocumentReader::readDocument(path.c_str(), root);
		}
		catch (std::exception ex)
		{
			LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("': ").append(ex.what()));
			delete root;
			return false;
		}
		this->fileName = path;
		char buffer[256]; memset(buffer, 0, sizeof(buffer));
		for (unsigned int rootNodeIndex = 0; rootNodeIndex < root->getNodeCount(); rootNodeIndex++)
		{
			const Node* layer1 = root->getNode(rootNodeIndex);
			if(layer1->getName().compare("vertexShader") == 0)
			{
				std::string shaderPath;
				for (unsigned int layer1ArgumentIndex = 0; layer1ArgumentIndex < layer1->getArgumentCount(); layer1ArgumentIndex++)
				{
					const Data* argument = layer1->getArgument(layer1ArgumentIndex);
					if (argument->getName().compare("path") == 0)
					{
						if (argument->getType() != DataTypes::STRING)
						{
							LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', '").append(layer1->getName()).append("' has invalid type for '").append(argument->getName()).append("'! Expected STRING."));
							delete root;
							return false;
						}
						shaderPath = ((DataString*)argument)->getDataAsString();
					}
				}
				if (shaderPath.empty())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', '").append(layer1->getName()).append("' is missing shader path argument."));
					delete root;
					return false;
				}
				std::fstream fileStream;
				fileStream.open(std::string(dir).append("\\").append(shaderPath), std::fstream::in);
				
				if(fileStream.fail())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(shaderPath)).append("', cannot read '").append(layer1->getName()).append("' file"));
					fileStream.close();
					delete root;
					return false;
				}
				while(!fileStream.eof())
				{
					fileStream.read(buffer, 255);
					vertexShaderSource.append(buffer);
					memset(buffer, 0, sizeof(buffer));
				}
				fileStream.close();
			}
			else if (layer1->getName().compare("fragmentShader") == 0)
			{
				std::string shaderPath;
				for (unsigned int layer1ArgumentIndex = 0; layer1ArgumentIndex < layer1->getArgumentCount(); layer1ArgumentIndex++)
				{
					const Data* argument = layer1->getArgument(layer1ArgumentIndex);
					if (argument->getName().compare("path") == 0)
					{
						if (argument->getType() != DataTypes::STRING)
						{
							LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', '").append(layer1->getName()).append("' has invalid type for '").append(argument->getName()).append("'! Expected STRING."));
							delete root;
							return false;
						}
						shaderPath = ((DataString*)argument)->getDataAsString();
					}
				}
				if (shaderPath.empty())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', '").append(layer1->getName()).append("' is missing shader path argument."));
					delete root;
					return false;
				}
				std::fstream fileStream;
				fileStream.open(std::string(dir).append("\\").append(shaderPath), std::fstream::in);

				if (fileStream.fail())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(shaderPath)).append("', cannot read '").append(layer1->getName()).append("' file"));
					fileStream.close();
					delete root;
					return false;
				}
				while (!fileStream.eof())
				{
					fileStream.read(buffer, 255);
					fragmentShaderSource.append(buffer);
					memset(buffer, 0, sizeof(buffer));
				}
				fileStream.close();
			}
		}
		delete root;
		return true;
	}
	bool Shader::compile(void)
	{
		LOGGER_WRITE(::Logger::DEBUG, std::string("Compiling Shader '").append(this->fileName).append("'"));
		GLint error = GL_FALSE;
		const char* vs_source = this->vertexShaderSource.c_str();
		const char* fs_source = this->fragmentShaderSource.c_str();

		//Create & compile vertex shader
		LOGGER_WRITE(::Logger::DEBUG, std::string("\tCompiling Vertex"));
		this->vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(this->vshader, 1, &vs_source, NULL);
		glCompileShader(this->vshader);
		glGetShaderiv(this->vshader, GL_COMPILE_STATUS, &error);
		if(!error)
		{
			GLint logSize;
			glGetShaderiv(this->vshader, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* log = new GLchar[logSize + 1];
			glGetShaderInfoLog(this->vshader, logSize, NULL, log);
			LOGGER_WRITE(::Logger::Priority::ERRORmsg, log);
			delete[logSize] log;
			return false;
		}

		//Create & compile fragment shader
		LOGGER_WRITE(::Logger::DEBUG, std::string("\tCompiling Fragment"));
		this->fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(this->fshader, 1, &fs_source, NULL);
		glCompileShader(this->fshader);
		glGetShaderiv(this->fshader, GL_COMPILE_STATUS, &error);
		if(!error)
		{
			GLint logSize;
			glGetShaderiv(this->fshader, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* log = new GLchar[logSize + 1];
			glGetShaderInfoLog(this->fshader, logSize, NULL, log);
			LOGGER_WRITE(::Logger::Priority::ERRORmsg, log);
			delete[logSize] log;
			return false;
		}

		//Create & Link GLSL programm
		LOGGER_WRITE(::Logger::DEBUG, std::string("\tLinking Shaders together"));
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vshader);
		glAttachShader(this->program, this->fshader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_LINK_STATUS, &error);
		if (!error) {
			GLint logSize;
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* log = new GLchar[logSize + 1];
			glGetProgramInfoLog(this->program, logSize, NULL, log);
			LOGGER_WRITE(::Logger::Priority::ERRORmsg, log);
			delete[logSize] log;
			return false;
		}
		compiled = true;
		return true;
	}
	void Shader::use(void)
	{
		if(!compiled) return;
		glUseProgram(this->program);
		for(unsigned int i = 0; i < this->vertexShaderAttributeLocations.size(); i++)
		{
			glEnableVertexAttribArray(this->vertexShaderAttributeLocations[i]);
		}
	}
	void Shader::unuse(void)
	{
		if(!compiled) return;
		for(unsigned int i = 0; i < this->vertexShaderAttributeLocations.size(); i++)
		{
			glDisableVertexAttribArray(this->vertexShaderAttributeLocations[i]);
		}
		glUseProgram(0);
	}
	
	bool Shader::setUniform1f(GLchar* varname, GLfloat v0, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform1f(loc, v0);
		
		return true;
	}
	bool Shader::setUniform2f(GLchar* varname, GLfloat v0, GLfloat v1, GLint index)
	{
	   if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  

		glUniform2f(loc, v0, v1); 

		return true;
	}
	bool Shader::setUniform3f(GLchar* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3f(loc, v0, v1, v2);
		return true;
	}
	bool Shader::setUniform4f(GLchar* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4f(loc, v0, v1, v2, v3);
		return true;
	}
	bool Shader::setUniform1i(GLchar* varname, GLint v0, GLint index)
	{ 
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;
		glUniform1i(loc, v0);
		
		return true;
	}
	bool Shader::setUniform2i(GLchar* varname, GLint v0, GLint v1, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform2i(loc, v0, v1);
		return true;
	}
	bool Shader::setUniform3i(GLchar* varname, GLint v0, GLint v1, GLint v2, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3i(loc, v0, v1, v2);
		return true;
	}
	bool Shader::setUniform4i(GLchar* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4i(loc, v0, v1, v2, v3);
		return true;
	}
	bool Shader::setUniform1ui(GLchar* varname, GLuint v0, GLint index)
	{ 
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform1uiEXT(loc, v0);
		
		return true;
	}
	bool Shader::setUniform2ui(GLchar* varname, GLuint v0, GLuint v1, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform2uiEXT(loc, v0, v1);
		return true;
	}
	bool Shader::setUniform3ui(GLchar* varname, GLuint v0, GLuint v1, GLuint v2, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3uiEXT(loc, v0, v1, v2);
		return true;
	}
	bool Shader::setUniform4ui(GLchar* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4uiEXT(loc, v0, v1, v2, v3);
		return true;
	}
	bool Shader::setUniform1fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform1fv(loc, count, value);
		return true;
	}
	bool Shader::setUniform2fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform2fv(loc, count, value);
		return true;
	}
	bool Shader::setUniform3fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3fv(loc, count, value);
		return true;
	}
	bool Shader::setUniform4fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4fv(loc, count, value);
		return true;
	}
	bool Shader::setUniform1iv(GLchar* varname, GLsizei count, GLint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform1iv(loc, count, value);
		return true;
	}
	bool Shader::setUniform2iv(GLchar* varname, GLsizei count, GLint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform2iv(loc, count, value);
		return true;
	}
	bool Shader::setUniform3iv(GLchar* varname, GLsizei count, GLint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3iv(loc, count, value);
		return true;
	}
	bool Shader::setUniform4iv(GLchar* varname, GLsizei count, GLint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4iv(loc, count, value);
		return true;
	}
	bool Shader::setUniform1uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform1uivEXT(loc, count, value);
		return true;
	}
	bool Shader::setUniform2uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform2uivEXT(loc, count, value);
		return true;
	}
	bool Shader::setUniform3uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform3uivEXT(loc, count, value);
		return true;
	}
	bool Shader::setUniform4uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniform4uivEXT(loc, count, value);
		return true;
	}
	bool Shader::setUniformMatrix2fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniformMatrix2fv(loc, count, transpose, value);
		return true;
	}
	bool Shader::setUniformMatrix3fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniformMatrix3fv(loc, count, transpose, value);
		return true;
	}
	bool Shader::setUniformMatrix4fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
	{
		if (!compiled) return false;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		CHECKOPENGLERROR
		glUniformMatrix4fv(loc, count, transpose, value);
		CHECKOPENGLERROR
		return true;
	}
	GLint Shader::GetUniformLocation(const GLchar *name)
	{
		//GLint id;
		//glGetIntegerv(GL_CURRENT_PROGRAM, &id);
		//LOGGER_WRITE(::Logger::DEBUG, std::string("CURRENT PROGRAMM WHILE LOOKUP '").append(std::to_string(id)).append("'"));

		GLint loc;
		loc = glGetUniformLocation(this->program, name);
		if (loc < 0) switch (loc)
		{
		//case 0: LOGGER_WRITE(::Logger::GL_ERROR, std::string("Error: Programm '").append(std::to_string(this->program)).append("' is unknown for OpenGL")); break;
		case -1: LOGGER_WRITE(::Logger::GL_ERROR, std::string("Error: can't find uniform variable '").append(name).append("'")); break;
		}
		return loc;
	}
	void Shader::getUniformfv(GLchar* varname, GLfloat* values, GLint index)
	{	 
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return;  
		glGetUniformfv(this->program, loc, values);
		
	}
	void Shader::getUniformiv(GLchar* varname, GLint* values, GLint index)
	{
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return;  
		
		glGetUniformiv(this->program, loc, values);
	}
	void Shader::getUniformuiv(GLchar* varname, GLuint* values, GLint index)
	{
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return;  
		
		glGetUniformuivEXT(this->program, loc, values);
	}
	void  Shader::BindAttribLocation(GLint index, GLchar* name)
	{
	   glBindAttribLocation(this->program, index, name);
	}
	bool Shader::setVertexAttrib1f(GLuint index, GLfloat v0)
	{
	   if (!compiled) return false;
	   glVertexAttrib1f(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
	{
	   if (!compiled) return false;
	   glVertexAttrib2f(index, v0, v1);
	   
	   return true;
	}
	bool Shader::setVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
	{
	   if (!compiled) return false;
	   
		glVertexAttrib3f(index, v0, v1, v2);
		
		return true;
	}
	bool Shader::setVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
	   if (!compiled) return false;
	   
	   glVertexAttrib4f(index, v0, v1, v2, v3);
	   
	   return true;
	}
	bool Shader::setVertexAttrib1d(GLuint index, GLdouble v0)
	{
	   if (!compiled) return false;
	   glVertexAttrib1d(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
	{
	   if (!compiled) return false;
	   glVertexAttrib2d(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
	{
	   if (!compiled) return false;
	   glVertexAttrib3d(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
	{
	   if (!compiled) return false;
	   glVertexAttrib4d(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1s(GLuint index, GLshort v0)
	{
	   if (!compiled) return false;
	   glVertexAttrib1s(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
	{
	   if (!compiled) return false;
	   glVertexAttrib2s(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
	{
	   if (!compiled) return false;
	   glVertexAttrib3s(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
	{
	   if (!compiled) return false;
	   glVertexAttrib4s(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
	{
	   if (!compiled) return false;
	   glVertexAttrib4Nub(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1i(GLuint index, GLint v0)
	{
	   if (!compiled) return false;
	   glVertexAttribI1iEXT(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2i(GLuint index, GLint v0, GLint v1)
	{
	   if (!compiled) return false;
	   glVertexAttribI2iEXT(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2)
	{
	   if (!compiled) return false;
	   glVertexAttribI3iEXT(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
	{
	   if (!compiled) return false;
	   glVertexAttribI4iEXT(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1ui(GLuint index, GLuint v0)
	{
	   if (!compiled) return false;
	   glVertexAttribI1uiEXT(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1)
	{
	   if (!compiled) return false;
	   glVertexAttribI2uiEXT(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2)
	{
	   if (!compiled) return false;
	   glVertexAttribI3uiEXT(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
	{
	   if (!compiled) return false;
	   glVertexAttribI4uiEXT(index, v0, v1, v2, v3);
	   return true;
	}
};