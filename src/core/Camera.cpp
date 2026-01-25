#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Camera::Camera() {
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;
    rotation[0] = 0.0f;
    rotation[1] = 0.0f;
    rotation[2] = 0.0f;
}

Camera::~Camera() {
}

void Camera::SetPosition(float x, float y, float z) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

void Camera::SetRotation(float pitch, float yaw, float roll) {
    rotation[0] = pitch;
    rotation[1] = yaw;
    rotation[2] = roll;


}

glm::mat4 Camera::GetViewMatrix(float* matrix) {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-position[0], -position[1], -position[2]));
    view = glm::rotate(view, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    if (matrix) {
        const float* ptr = glm::value_ptr(view);
        for (int i = 0; i < 16; ++i) {
            matrix[i] = ptr[i];
        }
    }
    return view;
}