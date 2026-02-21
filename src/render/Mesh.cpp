#include <render/Mesh.h>

Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_colorVBO(0),m_uvVBO(0), m_indexCount(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
	glGenBuffers(1, &m_colorVBO);
	glGenBuffers(1, &m_uvVBO);
}
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteBuffers(1, &m_uvVBO);
	glDeleteBuffers(1, &m_colorVBO);
}
void Mesh::loadFromFile(const char* filename)
{
	(void)filename;
}
void Mesh::Draw()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Init(const float* positions, size_t posCount, const float* colors, size_t colorCount, const float* uvs, size_t ucCount, const GLuint* indices, size_t indexCount)
{
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, posCount * sizeof(float), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, colorCount * sizeof(float), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_uvVBO);
	glBufferData(GL_ARRAY_BUFFER, ucCount * sizeof(float), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);
	m_indexCount = static_cast<GLuint>(indexCount);
	glBindVertexArray(0);




}

void Mesh::DrawLines()
{
	glBindVertexArray(m_VAO);
	glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}