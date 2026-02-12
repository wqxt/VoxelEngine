#include "TargetOutline.h"
#include "core/Camera.h"
#include "render/Render.h"
#include "render/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

TargetOutline::TargetOutline() {}

TargetOutline::~TargetOutline() {}

void TargetOutline::Init() 
{
    float vertices[] = {
        -0.5f,-0.5f,-0.5f, // 0
         0.5f,-0.5f,-0.5f, // 1
         0.5f, 0.5f,-0.5f, // 2
        -0.5f, 0.5f,-0.5f, // 3
        -0.5f,-0.5f, 0.5f, // 4
         0.5f,-0.5f, 0.5f, // 5
         0.5f, 0.5f, 0.5f, // 6
        -0.5f, 0.5f, 0.5f  // 7
    };


    float colors[] = 
    {
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f,
        1.f,1.f,1.f
    };


    GLuint indices[] = 
    {

        0,1,
        1,2,
        2,3,
        3,0,

        4,5,
        5,6,
        6,7,
        7,4,

        0,4,
        1,5,
        2,6,
        3,7
    };

    m_mesh.Init(vertices, 24, colors, 24, indices, 24);

}

void TargetOutline::SetTransform(float& posX, float& posY, float& posZ) 
{
	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;
}

void TargetOutline::Draw(const Camera& camera, Render& renderer, Shader& shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(m_posX, m_posY, m_posZ));
    model = glm::scale(model, glm::vec3(1.02f));
	renderer.DrawLines(shader, m_mesh, camera.GetViewMatrix(), camera.GetProjectionMatrix(), model);
}