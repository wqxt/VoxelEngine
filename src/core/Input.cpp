#include "Input.h"

bool Input::keys[1024] = { false };
bool Input::previousKeys[1024] = { false };
double Input::s_lastMouseX = 0.0;
double Input::s_lastMouseY = 0.0;
float Input::s_deltaX = 0.0f;
float Input::s_deltaY = 0.0f;
bool Input::s_firstMouse = true;



void Input::Init(GLFWwindow* window) 
{
	m_window = window;
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetCursorPosCallback(window, Input::MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Input::~Input() {}

bool Input::IsKeyPressed(int key) 
{
	return keys[key];
}

bool Input::IsMouseKeyPressed(int key) 
{
	return glfwGetMouseButton(m_window, key) == GLFW_PRESS;
}

void Input::GetMouseDelta(float& xoffset, float& yoffset) 
{
	xoffset = s_deltaX;
	yoffset = s_deltaY;
	s_deltaX = 0.0f;
	s_deltaY = 0.0f;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	(void)window;
	(void)scancode;
	(void)mods;

	if (action == GLFW_PRESS)
	{
		keys[key] = true;

	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}

}

void Input::MouseCallback(GLFWwindow* window, double xpos, double ypos) 
{
	(void)window;

	if (s_firstMouse) {
		s_lastMouseX = xpos;
		s_lastMouseY = ypos;
		s_firstMouse = false;
		return;
	}

	s_deltaX += static_cast<float>(xpos - s_lastMouseX);
	s_deltaY -= static_cast<float>(s_lastMouseY - ypos);
	s_lastMouseX = xpos;
	s_lastMouseY = ypos;
}