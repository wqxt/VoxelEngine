#pragma once
#include <glad/glad.h>

class Mesh {

public:
	Mesh();
	~Mesh();
	void loadFromFile(const char* filename);
	void Init(const float* positions, size_t posCount, const float* colors,
		size_t colorCount, const float* uvs, size_t ucCount, const GLuint* indices, size_t indexCount);
	void Draw();
	void DrawLines();

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
	GLuint m_uvVBO;
	GLuint m_colorVBO;
	GLuint m_indexCount;
};