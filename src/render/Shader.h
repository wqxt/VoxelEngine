#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader {
public:
	Shader();
	~Shader();
	void Load(const char* vertexSource, const char* fragmentSource);
	void LoadFromFiles(const char* vertexPath, const char* fragmentPath);
	void Use();
	void SetMat4(const char* name, const glm::mat4& mat);
	void SetShaderShade(const char* name, float value);

private:
	GLuint m_programID;
	GLuint CompileShader(const char* source, GLenum shaderType);

};