#include "Crosshair.h"
#include "core/Camera.h"
#include "render/Render.h"
#include "render/Shader.h"
#include <glm/glm.hpp>

Crosshair::Crosshair() {}
Crosshair::~Crosshair() {}

void Crosshair::Init() 
{
	float vertices[] = 
	{
		-0.01f, 0.f, 0.f,
		 0.01f, 0.f, 0.f,
		 0.f, -0.01f, 0.f,
		 0.f,  0.01f, 0.f
	};
	float colors[] = 
	{
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};

	float uvs[] = 
	{
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	GLuint indices[] = { 0, 1, 2, 3 };
	m_mesh.Init(vertices, 12, colors, 12,uvs,4, indices, 4);
}

void Crosshair::Draw(const Camera& camera, Render& renderer, Shader& shader) 
{
	glm::vec3 camPos = camera.GetPosition();
	glm::vec3 camFront = camera.GetFront();
	glm::vec3 camRight = glm::normalize(glm::cross(camFront, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 camUp = glm::normalize(glm::cross(camRight, camFront));
	glm::mat4 model = glm::mat4(1.f);

	model[0] = glm::vec4(camRight, 0.f);
	model[1] = glm::vec4(camUp, 0.f);
	model[2] = glm::vec4(-camFront, 0.f);
	model[3] = glm::vec4(camPos + camFront * 0.5f, 1.f);
	renderer.DrawLines(shader, m_mesh, camera.GetViewMatrix(), camera.GetProjectionMatrix(), model);
}