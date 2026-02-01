#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <render/Renderer.h>
#include <render/Mesh.h>
#include <render/Shader.h>
#include <chrono>

int main() {

	Window window(1600, 720, "VoxelEngine");
	GLFWwindow* glfwWindow = const_cast<GLFWwindow*>(window.GetGLFWWindow());

	Renderer renderer;
	renderer.Init();

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	};

	float colors[] = {
	1.0f, 0.0f, 0.0f,  // red (vertex 0)
	0.0f, 1.0f, 0.0f,  // green (vertex 1)
	0.0f, 0.0f, 1.0f,  // blue (vertex 2)
	1.0f, 1.0f, 0.0f,  // yellow (vertex 3)
	1.0f, 0.0f, 1.0f,  // magenta (vertex 4)
	0.0f, 1.0f, 1.0f,  // cyan (vertex 5)
	1.0f, 1.0f, 1.0f,  // white (vertex 6)
	0.5f, 0.5f, 0.5f,  // gray (vertex 7)
	};

	GLuint indices[] = {
		0, 1, 5, 5, 4, 0,  // left face
		2, 3, 7, 7, 6, 2,  // right face
		0, 2, 3, 3, 1, 0,  // back face
		4, 5, 7, 7, 6, 4,  // front face
		0, 4, 6, 6, 2, 0,  // bottom face
		1, 3, 7, 7, 5, 1   // top face
	};

	float gridVertices[] = {
		-4.f,-0.8f,-4.f, -3.f,-0.8f,-4.f, -2.f,-0.8f,-4.f, -1.f,-0.8f,-4.f,  0.f,-0.8f,-4.f,  1.f,-0.8f,-4.f,  2.f,-0.8f,-4.f,  3.f,-0.8f,-4.f,  4.f,-0.8f,-4.f,
		-4.f,-0.8f,-3.f, -3.f,-0.8f,-3.f, -2.f,-0.8f,-3.f, -1.f,-0.8f,-3.f,  0.f,-0.8f,-3.f,  1.f,-0.8f,-3.f,  2.f,-0.8f,-3.f,  3.f,-0.8f,-3.f,  4.f,-0.8f,-3.f,
		-4.f,-0.8f,-2.f, -3.f,-0.8f,-2.f, -2.f,-0.8f,-2.f, -1.f,-0.8f,-2.f,  0.f,-0.8f,-2.f,  1.f,-0.8f,-2.f,  2.f,-0.8f,-2.f,  3.f,-0.8f,-2.f,  4.f,-0.8f,-2.f,
		-4.f,-0.8f,-1.f, -3.f,-0.8f,-1.f, -2.f,-0.8f,-1.f, -1.f,-0.8f,-1.f,  0.f,-0.8f,-1.f,  1.f,-0.8f,-1.f,  2.f,-0.8f,-1.f,  3.f,-0.8f,-1.f,  4.f,-0.8f,-1.f,
		-4.f,-0.8f, 0.f, -3.f,-0.8f, 0.f, -2.f,-0.8f, 0.f, -1.f,-0.8f, 0.f,  0.f,-0.8f, 0.f,  1.f,-0.8f, 0.f,  2.f,-0.8f, 0.f,  3.f,-0.8f, 0.f,  4.f,-0.8f, 0.f,
		-4.f,-0.8f, 1.f, -3.f,-0.8f, 1.f, -2.f,-0.8f, 1.f, -1.f,-0.8f, 1.f,  0.f,-0.8f, 1.f,  1.f,-0.8f, 1.f,  2.f,-0.8f, 1.f,  3.f,-0.8f, 1.f,  4.f,-0.8f, 1.f,
		-4.f,-0.8f, 2.f, -3.f,-0.8f, 2.f, -2.f,-0.8f, 2.f, -1.f,-0.8f, 2.f,  0.f,-0.8f, 2.f,  1.f,-0.8f, 2.f,  2.f,-0.8f, 2.f,  3.f,-0.8f, 2.f,  4.f,-0.8f, 2.f,
		-4.f,-0.8f, 3.f, -3.f,-0.8f, 3.f, -2.f,-0.8f, 3.f, -1.f,-0.8f, 3.f,  0.f,-0.8f, 3.f,  1.f,-0.8f, 3.f,  2.f,-0.8f, 3.f,  3.f,-0.8f, 3.f,  4.f,-0.8f, 3.f,
		-4.f,-0.8f, 4.f, -3.f,-0.8f, 4.f, -2.f,-0.8f, 4.f, -1.f,-0.8f, 4.f,  0.f,-0.8f, 4.f,  1.f,-0.8f, 4.f,  2.f,-0.8f, 4.f,  3.f,-0.8f, 4.f,  4.f,-0.8f, 4.f
	};

	float gridColors[243];
	for (size_t i = 0; i < 81; ++i) {
		gridColors[i * 3] = 0.f;
		gridColors[i * 3 + 1] = 1.f;
		gridColors[i * 3 + 2] = 0.f;
	}

	GLuint gridIndices[] = {
		0,1, 1,2, 2,3, 3,4, 4,5, 5,6, 6,7, 7,8,
		9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,16, 16,17,
		18,19, 19,20, 20,21, 21,22, 22,23, 23,24, 24,25, 25,26,
		27,28, 28,29, 29,30, 30,31, 31,32, 32,33, 33,34, 34,35,
		36,37, 37,38, 38,39, 39,40, 40,41, 41,42, 42,43, 43,44,
		45,46, 46,47, 47,48, 48,49, 49,50, 50,51, 51,52, 52,53,
		54,55, 55,56, 56,57, 57,58, 58,59, 59,60, 60,61, 61,62,
		63,64, 64,65, 65,66, 66,67, 67,68, 68,69, 69,70, 70,71,
		72,73, 73,74, 74,75, 75,76, 76,77, 77,78, 78,79, 79,80,
		0,9, 9,18, 18,27, 27,36, 36,45, 45,54, 54,63, 63,72,
		1,10, 10,19, 19,28, 28,37, 37,46, 46,55, 55,64, 64,73,
		2,11, 11,20, 20,29, 29,38, 38,47, 47,56, 56,65, 65,74,
		3,12, 12,21, 21,30, 30,39, 39,48, 48,57, 57,66, 66,75,
		4,13, 13,22, 22,31, 31,40, 40,49, 49,58, 58,67, 67,76,
		5,14, 14,23, 23,32, 32,41, 41,50, 50,59, 59,68, 68,77,
		6,15, 15,24, 24,33, 33,42, 42,51, 51,60, 60,69, 69,78,
		7,16, 16,25, 25,34, 34,43, 43,52, 52,61, 61,70, 70,79,
		8,17, 17,26, 26,35, 35,44, 44,53, 53,62, 62,71, 71,80
	};

	Shader shader;
	shader.LoadFromFiles("../../assets/shaders/default.vert", "../../assets/shaders/default.frag");


	Mesh cubeMesh;
	cubeMesh.Init(vertices, sizeof(vertices) / sizeof(float),
		colors, sizeof(colors) / sizeof(float),
		indices, sizeof(indices) / sizeof(GLuint));

	Mesh gridMesh;
	gridMesh.Init(gridVertices, sizeof(gridVertices) / sizeof(float),
		gridColors, sizeof(gridColors) / sizeof(float),
		gridIndices, sizeof(gridIndices) / sizeof(GLuint));



	Input input;
	input.Init(glfwWindow);

	window.PollEvents();

	Camera camera;
	camera.Init();

	auto lastTime = std::chrono::high_resolution_clock::now();
	float rotationAngle = 0.f;
	while (!window.ShouldClose()) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		window.PollEvents();


		camera.Update(&input, deltaTime);

		rotationAngle += deltaTime * 50.f;
		glm::mat4 model = glm::mat4(1.f);
		model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(1, 1, 0));

		renderer.Clear();
		renderer.DrawLines(shader, gridMesh, camera.GetViewMatrix(),
			camera.GetProjectionMatrix(), glm::mat4(1.f));
		renderer.Draw(shader, cubeMesh, camera.GetViewMatrix(),
			camera.GetProjectionMatrix(), model);
		window.SwapBuffer();
	}
	return 0;
}