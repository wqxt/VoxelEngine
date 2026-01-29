#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>
#include "core/Input.h"

class Camera {
public:
	Camera();
	~Camera();

	void Init();
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw);
	void SetFOV(float fovDegrees);
	void SetMovementSpeed(float speed);
	void UpdateVectors();
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void Update(Input* input,float deltaTime);

private:
	glm::vec3 m_position;

	float m_pitch;
	float m_yaw;

	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;


	float m_movementSpeed;
	float m_mouseSensitivity;

	float m_fov;
	float m_aspect;
	float m_far;
	float m_near;

};