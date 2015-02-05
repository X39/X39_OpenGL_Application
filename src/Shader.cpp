﻿#include "Shader.h"
#include "globals.h"
#include "dotX39\SettingsDocument.h"
#include "dotX39\ISettingsFileHandler.h"
#include "CommandHandler\CommandHandler.h"

#include <iostream>

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
		ISettingsFileHandler fh = ISettingsFileHandler(path.c_str());
		SettingsDocument* sd = fh.load();
		
		std::string dir = path.substr(0, path.find_last_of("/\\"));

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		if(sd == NULL)
		{
			LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', cannot parse file"));
			return false;
		}
		SettingsNode root = *(sd->getRootNode());
		for(unsigned int rootIndex = 0; rootIndex < root.childs.size(); rootIndex++)
		{
			SettingsNode layer1 = *(root.childs[rootIndex]);
			if(layer1.boundOptions.size() <= 0 || layer1.boundOptions.size() > 1)
			{
				LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', ").append(layer1.getName()).append(" has either no name option or more then one option"));
				return false;
			}
			SettingsOption name = *(layer1.boundOptions[0]);
			if(strcmp(name.getName(), "relativePath") != 0)
			{
				LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(path).append("', ").append(layer1.getName()).append(" path option is not named 'relativePath'"));
				return false;
			}

			if(strcmp(layer1.getName(), "vertexShader") == 0)
			{
				std::fstream fileStream;
				fileStream.open(std::string(dir).append("\\").append(name.getValue()), std::fstream::in);
				
				if(fileStream.fail())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(name.getValue())).append("', cannot read vertexShader file"));
					fileStream.close();
					return false;
				}
				while(!fileStream.eof())
				{
					fileStream.read(buffer, 255);
					vertexShaderSource.append(buffer);
					memset(buffer, 0, sizeof(buffer));
				}
				fileStream.close();
				for(unsigned int layer1Index = 0; layer1Index < layer1.boundOptions.size(); layer1Index++)
				{
					SettingsOption opt = *(layer1.boundOptions[layer1Index]);
					if(strcmp(opt.getName(), "attributes") == 0)
					{
						double d = 0;
						if(!::CommandHandler::convAsciiCharToDouble(opt.getValue(), &d))
						{
							LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(name.getValue())).append("'.").append(" Please check '").append(opt.getName()).append("'. Cannot read vertexShader file"));
							fileStream.close();
							return false;
						}
						vertexShaderAttributeLocations.push_back((unsigned int)d);
					}
				}
			}
			else if(strcmp(layer1.getName(), "fragmentShader") == 0)
			{
				std::fstream fileStream;
				fileStream.open(std::string(dir).append("\\").append(name.getValue()), std::fstream::in);
				if(fileStream.fail())
				{
					LOGGER_WRITE(::Logger::ERRORmsg, std::string("\terror while parsing '").append(std::string(dir).append("\\").append(name.getValue())).append("', cannot read vertexShader file"));
					fileStream.close();
					return false;
				}
				while(!fileStream.eof())
				{
					fileStream.read(buffer, 255);
					fragmentShaderSource.append(buffer);
					memset(buffer, 0, sizeof(buffer));
				}
				fileStream.close();
			}
		}
		return true;
	}
	bool Shader::compile(void)
	{
		GLint error = GL_FALSE;
		const char* vs_source = this->vertexShaderSource.c_str();
		const char* fs_source = this->fragmentShaderSource.c_str();

		//Create & compile vertex shader
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
		if (!compiled) return true;
		
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
	   if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
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
		if (!compiled) return true;
		
		GLint loc;
		if (varname)
		   loc = GetUniformLocation(varname);
		else
		   loc = index;
		if (loc==-1) 
		   return false;  
		
		glUniformMatrix4fv(loc, count, transpose, value);
		return true;
	}
	GLint Shader::GetUniformLocation(const GLchar *name)
	{
		GLint loc;
		loc = glGetUniformLocation(this->program, name);
		if (loc == -1) 
		{
			LOGGER_WRITE(::Logger::GL_ERROR, std::string("Error: can't find uniform variable '").append(name).append("'"));
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
	   if (!compiled) return true;
	   glVertexAttrib1f(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
	{
	   if (!compiled) return true;
	   glVertexAttrib2f(index, v0, v1);
	   
	   return true;
	}
	bool Shader::setVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
	{
	   if (!compiled) return true;
	   
		glVertexAttrib3f(index, v0, v1, v2);
		
		return true;
	}
	bool Shader::setVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
	   if (!compiled) return true;
	   
	   glVertexAttrib4f(index, v0, v1, v2, v3);
	   
	   return true;
	}
	bool Shader::setVertexAttrib1d(GLuint index, GLdouble v0)
	{
	   if (!compiled) return true;
	   glVertexAttrib1d(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
	{
	   if (!compiled) return true;
	   glVertexAttrib2d(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
	{
	   if (!compiled) return true;
	   glVertexAttrib3d(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
	{
	   if (!compiled) return true;
	   glVertexAttrib4d(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1s(GLuint index, GLshort v0)
	{
	   if (!compiled) return true;
	   glVertexAttrib1s(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
	{
	   if (!compiled) return true;
	   glVertexAttrib2s(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
	{
	   if (!compiled) return true;
	   glVertexAttrib3s(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
	{
	   if (!compiled) return true;
	   glVertexAttrib4s(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
	{
	   if (!compiled) return true;
	   glVertexAttrib4Nub(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1i(GLuint index, GLint v0)
	{
	   if (!compiled) return true;
	   glVertexAttribI1iEXT(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2i(GLuint index, GLint v0, GLint v1)
	{
	   if (!compiled) return true;
	   glVertexAttribI2iEXT(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2)
	{
	   if (!compiled) return true;
	   glVertexAttribI3iEXT(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
	{
	   if (!compiled) return true;
	   glVertexAttribI4iEXT(index, v0, v1, v2, v3);
	   return true;
	}
	bool Shader::setVertexAttrib1ui(GLuint index, GLuint v0)
	{
	   if (!compiled) return true;
	   glVertexAttribI1uiEXT(index, v0);
	   return true;
	}
	bool Shader::setVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1)
	{
	   if (!compiled) return true;
	   glVertexAttribI2uiEXT(index, v0, v1);
	   return true;
	}
	bool Shader::setVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2)
	{
	   if (!compiled) return true;
	   glVertexAttribI3uiEXT(index, v0, v1, v2);
	   return true;
	}
	bool Shader::setVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
	{
	   if (!compiled) return true;
	   glVertexAttribI4uiEXT(index, v0, v1, v2, v3);
	   return true;
	}
};