#include "shaders.h"


ShaderBase::ShaderBase(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	string vertexCode;
	string fragmentCode;
	string geometryCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream gShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	gShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		fShaderFile.open(fragmentPath);
		vShaderFile.open(vertexPath);
		
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			stringstream gShaderStream;

			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();

			geometryCode = gShaderStream.str();
		}
	}
	catch (ifstream::failure e)
	{
		string infoOutput = "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
		cout << infoOutput << std::endl;
		LogInfo(infoOutput);
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader, fragmentShader, geometryShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, "FRAGMENT");

	if (geometryPath != nullptr)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &gShaderCode, NULL);
		glCompileShader(geometryShader);
		checkCompileErrors(geometryShader, "GEOMETRY");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometryShader);

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if(geometryPath != nullptr)
		glDeleteShader(geometryShader);

}


ShaderBase::~ShaderBase()
{
}

void ShaderBase::checkCompileErrors(GLuint shaderORprogram, std::string type)
{
	GLint success;
	string infoOutput;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderORprogram, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderORprogram, 1024, NULL, infoLog);
			infoOutput = "ERROR::SHADER_COMPILATION_ERROR of type : " + type + "\n" + infoLog + "\n-- -------------------------------------------------- - -- ";
			cout << infoOutput << std::endl;
			LogInfo(infoOutput);
		}
	}
	else
	{
		glGetProgramiv(shaderORprogram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderORprogram, 1024, NULL, infoLog);
			infoOutput = "ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n -- --------------------------------------------------- -- ";
			cout << infoOutput << endl;
			LogInfo(infoOutput);
		}
	}
}

void ShaderBase::use() const
{
	glUseProgram(ID);
}

GLint ShaderBase::GetUniformLocation(const string &name) const
{
	return glGetUniformLocation(ID, name.c_str());
}
GLint ShaderBase::GetAttribLocation(const string &name) const
{
	return glGetAttribLocation(ID, name.c_str());
}

//---------set Uniforms-------------
void ShaderBase::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void ShaderBase::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderBase::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void ShaderBase::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderBase::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void ShaderBase::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderBase::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void ShaderBase::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void ShaderBase::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void ShaderBase::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderBase::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void ShaderBase::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}