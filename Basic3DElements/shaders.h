#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <GL\glew.h>
#include <stb_image.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "winContext.h"


using namespace std;
using namespace glm;

class ShaderBase
{
private:
	void checkCompileErrors(GLuint shader, std::string type);
public:
	unsigned int ID;

	ShaderBase(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	~ShaderBase();
	void use() const;

	//Methods for setting the Uniform Variables
	void setBool(const string &name, bool value) const;
	void setInt(const string &name, int value) const;
	void setFloat(const string &name, float value) const;
	void setVec2(const string &name, const vec2 &value) const;
	void setVec2(const string &name, float x, float y) const;
	void setVec3(const string &name, const vec3 &value) const;
	void setVec3(const string &name, float x, float y, float z) const;
	void setVec4(const string &name, const vec4 &value) const;
	void setVec4(const string &name, float x, float y, float z, float w) const;
	void setMat2(const string &name, const mat2 &mat) const;
	void setMat3(const string &name, const mat3 &mat) const;
	void setMat4(const string &name, const mat4 &mat) const;

	//get Attributes
	GLint GetUniformLocation(const string &name) const;
	GLint GetAttribLocation(const string &name) const;
};

