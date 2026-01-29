#include "Input.h"

bool Input::keys[1024] = { false };
bool Input::previousKeys[1024] = { false };



void Input::Init(GLFWwindow* window) {
	m_window = window;
	glfwSetKeyCallback(window, Input::KeyCallback);

}

Input::~Input() {

}


bool Input::IsKeyPressed(int key) {

	return keys[key];
}

void Input::GetMouseDelta(float& xoffset, float& yoffset) {
}

void  Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS)
	{
		keys[key] = true;

	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}

}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) {


}