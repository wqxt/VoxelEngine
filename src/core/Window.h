#pragma once
#include <GLFW/glfw3.h>

class Window {

public:
	Window(int width, int height, const char* title);
	~Window();

	void SwapBuffer();
	void PollEvents();
	void Close();

	int GetWidth() const { return m_widgth; }
	int GetHeigth() const { return m_heigth; }
	GLFWwindow* GetGLFWWindow() const { return m_window; }

	bool ShouldClose() const;

private:

	GLFWwindow* m_window;
	int m_widgth;
	int m_heigth;
};