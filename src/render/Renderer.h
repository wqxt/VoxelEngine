#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
	void Clear();
    void Draw(Shader& shader, Mesh& mesh, const glm::mat4& view, 
        const glm::mat4& projection, const glm::mat4& model);
};