#include <glad/glad.h>  
#include <iostream>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <render/Renderer.h>
#include <chrono>

int main() {
	Window window(1600, 720, "VoxelEngine");

	GLFWwindow* glfwWindow = const_cast<GLFWwindow*>(window.GetGLFWWindow());

	Input input;
	input.Init(glfwWindow);

	window.PollEvents();

	Renderer renderer;
	renderer.Init();

	Camera camera;
	camera.Init();

	auto lastTime = std::chrono::high_resolution_clock::now();

	while (!window.ShouldClose()) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		window.PollEvents();
		//input.Update(deltaTime);

		camera.Update(&input,deltaTime);
		renderer.Update(deltaTime);
		renderer.Clear();
		renderer.DrawCube(camera);
		window.SwapBuffer();
	}

	return 0;
}