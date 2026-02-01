#include "Renderer.h"
#include "core/Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//const char* vertexShaderSource;
//const char* fragmentShaderSource;

Renderer::Renderer() : m_shaderProgram(), m_VAO(0), m_VBO(0), m_EBO(0), m_indexCount(0), m_rotationAngle(0.0f), m_colorVBO(0) {


}

Renderer::~Renderer() {
	if (m_VBO != 0) {
		glDeleteBuffers(1, &m_VBO);
	}
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
	}
	if (m_EBO != 0) {
		glDeleteBuffers(1, &m_EBO);
	}

	if (m_shaderProgram != 0) {
		glDeleteProgram(m_shaderProgram);
	}

	if (m_colorVBO != 0) {
		glDeleteBuffers(1, &m_colorVBO);
	}
}



void Renderer::Init() {

	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//m_shaderProgram = CompileShader(vertexShaderSource, fragmentShaderSource);
	//SetupCubeData();

}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}




void Renderer::SetupCubeData() {

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, 0.5f, -0.5f,
	 0.5f, -0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	};

	float colors[] = {
	1.0f, 0.0f, 0.0f,  // red (vertex 0)
	0.0f, 1.0f, 0.0f,  // green (vertex 1)
	0.0f, 0.0f, 1.0f,  // blue (vertex 2)
	1.0f, 1.0f, 0.0f,  // yellow (vertex 3)
	1.0f, 0.0f, 1.0f,  // magenta (vertex 4)
	0.0f, 1.0f, 1.0f,  // cyan (vertex 5)
	1.0f, 1.0f, 1.0f,  // white (vertex 6)
	0.5f, 0.5f, 0.5f,  // gray (vertex 7)
	};

	GLuint indices[] = {
		0, 1, 5, 5, 4, 0,  // left face
		2, 3, 7, 7, 6, 2,  // right face
		0, 2, 3, 3, 1, 0,  // back face
		4, 5, 7, 7, 6, 4,  // front face
		0, 4, 6, 6, 2, 0,  // bottom face
		1, 3, 7, 7, 5, 1   // top face
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_VBO); // create VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); // activate VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // send data to VBO buffer in GPU
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_indexCount = 36;

	glBindVertexArray(0);
}


void Renderer::Draw(const Camera& camera) {

}

void Renderer::Update(float deltaTime) {
	m_rotationAngle += deltaTime * 50.0f; // Rotate 50 degrees per second
	if (m_rotationAngle > 360.0f)
		m_rotationAngle -= 360.0f;
}