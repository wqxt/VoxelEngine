#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Render {
public:
    Render();
    ~Render();

    void Init();
	void Clear();
	void Draw(Shader& shader, Mesh& mesh, const glm::mat4& view,
		const glm::mat4& projection, const glm::mat4& model);
	void DrawLines(Shader& shader, Mesh& mesh, const glm::mat4& view, const glm::mat4& projection, const glm::mat4& model);
};