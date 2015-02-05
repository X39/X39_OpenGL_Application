#pragma once
#include <string>
#include <vector>

typedef unsigned int GLuint;
typedef int GLint;
typedef char GLchar;
typedef float GLfloat;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef double GLdouble;
typedef int GLsizei;
namespace X39
{
	class Shader
	{
		private:
			GLuint program;
			GLuint vshader;
			GLuint fshader;
			std::string vertexShaderSource;
			std::vector<unsigned int> vertexShaderAttributeLocations;
			std::string fragmentShaderSource;
			bool compiled;

		public:
			Shader(void);
			~Shader(void);

			bool load(std::string path);
			bool compile(void);
			void use(void);
			void unuse(void);

			bool setUniform1f(GLchar* varname, GLfloat v0, GLint index);
			bool setUniform2f(GLchar* varname, GLfloat v0, GLfloat v1, GLint index);
			bool setUniform3f(GLchar* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index);
			bool setUniform4f(GLchar* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index);
			bool setUniform1i(GLchar* varname, GLint v0, GLint index);
			bool setUniform2i(GLchar* varname, GLint v0, GLint v1, GLint index);
			bool setUniform3i(GLchar* varname, GLint v0, GLint v1, GLint v2, GLint index);
			bool setUniform4i(GLchar* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index);
			bool setUniform1ui(GLchar* varname, GLuint v0, GLint index);
			bool setUniform2ui(GLchar* varname, GLuint v0, GLuint v1, GLint index);
			bool setUniform3ui(GLchar* varname, GLuint v0, GLuint v1, GLuint v2, GLint index);
			bool setUniform4ui(GLchar* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index);
			bool setUniform1fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index);
			bool setUniform2fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index);
			bool setUniform3fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index);
			bool setUniform4fv(GLchar* varname, GLsizei count, GLfloat *value, GLint index);
			bool setUniform1iv(GLchar* varname, GLsizei count, GLint *value, GLint index);
			bool setUniform2iv(GLchar* varname, GLsizei count, GLint *value, GLint index);
			bool setUniform3iv(GLchar* varname, GLsizei count, GLint *value, GLint index);
			bool setUniform4iv(GLchar* varname, GLsizei count, GLint *value, GLint index);
			bool setUniform1uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index);
			bool setUniform2uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index);
			bool setUniform3uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index);
			bool setUniform4uiv(GLchar* varname, GLsizei count, GLuint *value, GLint index);
			bool setUniformMatrix2fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index);
			bool setUniformMatrix3fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index);
			bool setUniformMatrix4fv(GLchar* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index);
			GLint GetUniformLocation(const GLchar *name);
			void getUniformfv(GLchar* varname, GLfloat* values, GLint index);
			void getUniformiv(GLchar* varname, GLint* values, GLint index);
			void getUniformuiv(GLchar* varname, GLuint* values, GLint index);
			void  BindAttribLocation(GLint index, GLchar* name);
			bool setVertexAttrib1f(GLuint index, GLfloat v0);
			bool setVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1);
			bool setVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);
			bool setVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
			bool setVertexAttrib1d(GLuint index, GLdouble v0);
			bool setVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1);
			bool setVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2);
			bool setVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
			bool setVertexAttrib1s(GLuint index, GLshort v0);
			bool setVertexAttrib2s(GLuint index, GLshort v0, GLshort v1);
			bool setVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2);
			bool setVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3);
			bool setVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3);
			bool setVertexAttrib1i(GLuint index, GLint v0);
			bool setVertexAttrib2i(GLuint index, GLint v0, GLint v1);
			bool setVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2);
			bool setVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);
			bool setVertexAttrib1ui(GLuint index, GLuint v0);
			bool setVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1);
			bool setVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2);
			bool setVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	};

};