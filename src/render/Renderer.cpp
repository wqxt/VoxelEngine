#include "Renderer.h"
#include "core/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(){}

Renderer::~Renderer() {}

void Renderer::Init() {

	gladLoadGL();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(Shader& shader, Mesh& mesh, const glm::mat4& view,
	const glm::mat4& projection, const glm::mat4& model) {

	shader.Use();
	shader.SetMat4("view", view);
	shader.SetMat4("model", model);
	shader.SetMat4("projection", projection);
	mesh.Draw();
}

void Renderer::DrawLines(Shader& shader, Mesh& mesh, const glm::mat4& view,
	const glm::mat4& projection, const glm::mat4& model) {
	shader.Use();
	shader.SetMat4("view", view);
	shader.SetMat4("model", model);
	shader.SetMat4("projection", projection);
	mesh.DrawLines();
}