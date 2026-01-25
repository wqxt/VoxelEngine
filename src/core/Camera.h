#pragma once
#include <glm/ext/matrix_float4x4.hpp>

class Camera {

public:
	Camera();
	~Camera();
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	glm::mat4 GetViewMatrix(float* matrix);

private:
	float position[3];
	float rotation[3];
};