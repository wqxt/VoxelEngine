#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

Camera::Camera() 
{
	m_position = glm::vec3(0.0f, 0.0f, 3.0f);
	m_pitch = 0.0f;
	m_yaw = -90.0f;
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	m_movementSpeed = 2.0f;
	m_mouseSensitivity = 0.1f;
	m_fov = 45.0f;
	m_aspect = 1600.0f / 720.0f;
	m_near = 0.1f;
	m_far = 100.0f;
	m_right = normalize(cross(m_front, m_worldUp));
	m_up = normalize(cross(m_right, m_front));
}

void Camera::Init() 
{


	Camera::SetPosition(0.0f, 0.0f, 3.0f);
	Camera::SetRotation(0.0f, -90.0f);
	Camera::UpdateVectors();

}
Camera::~Camera() 
{
}

void Camera::SetPosition(float x, float y, float z) 
{
	m_position = glm::vec3(x, y, z);
}

void Camera::SetRotation(float pitch, float yaw) 
{
	m_pitch = pitch;
	m_yaw = yaw;

	Camera::UpdateVectors();
}

void Camera::UpdateVectors() 
{

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));  
	m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::SetFOV(float fovDegrees) 
{
	m_fov = fovDegrees;
}
void Camera::SetMovementSpeed(float speed) 
{
	m_movementSpeed = speed;
}

glm::mat4 Camera::GetViewMatrix() const 
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const 
{
	return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

glm::vec3 Camera::GetPosition() const 
{
	return m_position;
}

glm::vec3 Camera::GetFront() const 
{
	return m_front;
}

void Camera::Update(Input* input,float deltaTime) 
{

	if (input->IsKeyPressed(GLFW_KEY_W))
	{
		m_position += m_front * m_movementSpeed * deltaTime;
	}
	if (input->IsKeyPressed(GLFW_KEY_S))
	{
		m_position -= m_front * m_movementSpeed * deltaTime;
	}
	if (input->IsKeyPressed(GLFW_KEY_A))
	{
		m_position -= m_right * m_movementSpeed * deltaTime;
	}
	if (input->IsKeyPressed(GLFW_KEY_D))
	{
		m_position += m_right * m_movementSpeed * deltaTime;
	}
	if (input-> IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		m_position += m_worldUp * m_movementSpeed * deltaTime;
	}
	if (input -> IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		m_position -= m_worldUp * m_movementSpeed * deltaTime;
	}

	float dx, dy;

	input->GetMouseDelta(dx, dy);
	m_yaw += dx * m_mouseSensitivity;
	m_pitch -= dy * m_mouseSensitivity;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	else if ( m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}


	Camera::UpdateVectors();
}
