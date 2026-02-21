#include "Shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : m_programID(0) {}

Shader :: ~Shader() 
{
	if (m_programID != 0) {
		glDeleteProgram(m_programID);
	}
}


void Shader::SetShaderShade(const char* name, float value)
{
	GLint loc = glGetUniformLocation(m_programID, name);
	glUniform1f(loc, value);
}

void Shader::Load(const char* vertexSource, const char* fragmentSource)
{

	GLuint vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	if (vertexShader == 0)
	{
		glDeleteShader(fragmentShader); // delete fragment shader if vertex shader compilation failed
		std::cerr << "ERROR::SHADER::PROGRAM_NOT_LOADED" << std::endl;
		return;
	}

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader); // delete vertex shader if fragment shader compilation failed
		std::cerr << "ERROR::SHADER::PROGRAM_NOT_LOADED" << std::endl;
		return;
	}


	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int linkSuccess;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &linkSuccess);

	if (!linkSuccess)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(m_programID);
		m_programID = 0;
		return;
	}

}

void Shader::LoadFromFiles(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vFile(vertexPath);
	if (!vFile) {
		std::cerr << "ERROR::SHADER::FILE_NOT_OPEN " << vertexPath << std::endl;
		return;
	}
	std::stringstream vStream;
	vStream << vFile.rdbuf();
	std::string vertexSource = vStream.str();
	vFile.close();

	std::ifstream fFile(fragmentPath);
	if (!fFile) {
		std::cerr << "ERROR::SHADER::FILE_NOT_OPEN " << fragmentPath << std::endl;
		return;
	}
	std::stringstream fStream;
	fStream << fFile.rdbuf();
	std::string fragmentSource = fStream.str();
	fFile.close();

	Load(vertexSource.c_str(), fragmentSource.c_str());
}


void Shader::Use() 
{
	if( m_programID == 0) {
		std::cerr << "ERROR::SHADER::PROGRAM_NOT_LOADED" << std::endl;
		return;
	}
	glUseProgram(m_programID);
}

void Shader::SetMat4(const char* name, const glm::mat4& mat) 
{
	GLint location = glGetUniformLocation(m_programID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}



GLuint Shader::CompileShader(const char* source, GLenum shaderType) 
{

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];	
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}