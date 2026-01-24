#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Init();
    void Clear();
    void DrawCube();
    void Update(float deltaTime);
    void Present();

private:
    GLuint m_shaderProgram;
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_indexCount;
    float m_rotationAngle;

    GLuint CompileShader(const char* vertexSrc, const char* fragmentSrc);
    void SetupCubeData();
};