#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <render/Renderer.h>
#include <render/Mesh.h>
#include <render/Shader.h>
#include <voxel/Voxel.h>
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
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,
	};

	GLuint indices[] = {
		0, 1, 5, 5, 4, 0,
		2, 3, 7, 7, 6, 2,
		0, 2, 3, 3, 1, 0,
		4, 5, 7, 7, 6, 4,
		0, 4, 6, 6, 2, 0,
		1, 3, 7, 7, 5, 1
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

	Voxel voxel;
	int vx = 0, vy = 0, vz = 0;
	voxel.SetPosition(vx, vy, vz);

	Input input;
	input.Init(glfwWindow);

	window.PollEvents();

	Camera camera;
	camera.Init();

	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!window.ShouldClose()) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		window.PollEvents();
		camera.Update(&input, deltaTime);

		renderer.Clear();
		for (int tz = 0; tz < gridTilesPerAxis; ++tz)
			for (int tx = 0; tx < gridTilesPerAxis; ++tx) {
				glm::mat4 tileModel = glm::translate(glm::mat4(1.f),
					glm::vec3(tx - gridHalf, 0.f, tz - gridHalf));
				renderer.DrawLines(shader, gridTileMesh, camera.GetViewMatrix(),
					camera.GetProjectionMatrix(), tileModel);
			}

		glm::mat4 voxelModel = glm::translate(glm::mat4(1.f), glm::vec3(voxel.GetPosition()));
		renderer.Draw(shader, cubeMesh, camera.GetViewMatrix(),
			camera.GetProjectionMatrix(), voxelModel);
		window.SwapBuffer();
	}
	return 0;
}