#pragma once
#include <glad/glad.h>

class Mesh {

public:
	Mesh();
	~Mesh();
	void loadFromFile(const char* filename);
	void Init(const float* positions, size_t posCount, const float* colors, 
		size_t colorCount, const GLuint* indices, size_t indexCount);
	void Draw();

private:
	GLuint m_VAO; 
	GLuint m_VBO; 
	GLuint m_EBO;
	GLuint m_colorVBO;
	GLuint m_indexCount;

};