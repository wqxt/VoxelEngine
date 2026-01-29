#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() {
}

Camera::~Camera() {
}

void Camera::SetPosition(float x, float y, float z) {
}

void Camera::SetRotation(float pitch, float yaw) {
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::mat4(1.0f);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::mat4(1.0f);
}

void Camera::Update(float deltaTime) {

}
