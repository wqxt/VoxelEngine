#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <render/Renderer.h>
#include <render/Mesh.h>
#include <render/Shader.h>
#include <chrono>
#include <vector>

static void buildGridTile(int segments, float y,
	std::vector<float>& outVertices, std::vector<float>& outColors, std::vector<GLuint>& outIndices)
{
	const int side = segments + 1;
	const int vertexCount = side * side;
	outVertices.reserve(vertexCount * 3);
	outColors.reserve(vertexCount * 3);

	for (int iz = 0; iz < side; ++iz)
	{
		for (int ix = 0; ix < side; ++ix)
		{
			outVertices.push_back(ix / (float)segments);
			outVertices.push_back(y);
			outVertices.push_back(iz / (float)segments);
			outColors.push_back(0.f);
			outColors.push_back(1.f);
			outColors.push_back(0.f);
		}
	}

	for (int iz = 0; iz < side; ++iz)
	{
		for (int ix = 0; ix < segments; ++ix)
		{
			outIndices.push_back(iz * side + ix);
			outIndices.push_back(iz * side + ix + 1);
		}

	}

	for (int ix = 0; ix < side; ++ix)
	{
		for (int iz = 0; iz < segments; ++iz)
		{
			outIndices.push_back(iz * side + ix);
			outIndices.push_back((iz + 1) * side + ix);
		}
	}
}

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

	const int gridTileSegments = 4;
	const float gridY = -0.8f;
	std::vector<float> tileVertices, tileColors;
	std::vector<GLuint> tileIndices;
	buildGridTile(gridTileSegments, gridY, tileVertices, tileColors, tileIndices);

	Shader shader;
	shader.LoadFromFiles("../../assets/shaders/default.vert", "../../assets/shaders/default.frag");

	Mesh cubeMesh;
	cubeMesh.Init(vertices, sizeof(vertices) / sizeof(float),
		colors, sizeof(colors) / sizeof(float),
		indices, sizeof(indices) / sizeof(GLuint));

	Mesh gridTileMesh;
	gridTileMesh.Init(tileVertices.data(), tileVertices.size(),
		tileColors.data(), tileColors.size(),
		tileIndices.data(), tileIndices.size());

	const int gridTilesPerAxis = 9;
	const int gridHalf = gridTilesPerAxis / 2;



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

		for (int tz = 0; tz < gridTilesPerAxis; ++tz)
		{
			for (int tx = 0; tx < gridTilesPerAxis; ++tx)
			{
				glm::mat4 tileModel = glm::translate(glm::mat4(1.f),
				glm::vec3(tx - gridHalf, 0.f, tz - gridHalf));
				renderer.DrawLines(shader, gridTileMesh, camera.GetViewMatrix(),
				camera.GetProjectionMatrix(), tileModel);
			}
		}

		renderer.Draw(shader, cubeMesh, camera.GetViewMatrix(),
			camera.GetProjectionMatrix(), model);
		window.SwapBuffer();
	}
	return 0;
}