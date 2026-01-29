#pragma once
#include <GLFW/glfw3.h>

class Input {
public:
	void Init(GLFWwindow* window);
	~Input();
	//void Update(float deltaTime);
	bool IsKeyPressed(int key);
	void GetMouseDelta(float& xoffset, float& yoffset);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
	GLFWwindow* m_window;

	static bool keys[1024];
	static bool previousKeys[1024];



};