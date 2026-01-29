#pragma once
#include <glm/ext/matrix_float4x4.hpp>

class Camera {
public:
	Camera();
	~Camera();
	
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw);
	
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	
	void Update(float deltaTime);

private:

};
