#include "core/Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title) 
: m_widgth(width), m_heigth(m_widgth){

    if(glfwInit() == false)
    {

        std::cerr << "Failed to init GLFW " << std::endl;
        return;
    }

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);

    if(m_window == NULL)
    {

        std::cerr << "Falied to create window"<< std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
}

Window::~Window() 
{
    if (m_window ) 
    {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
}

void Window::SwapBuffer()
{
	glfwSwapBuffers(m_window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_window, 1);
}