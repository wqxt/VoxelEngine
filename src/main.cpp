#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <render/Renderer.h>
#include <render/Mesh.h>
#include <render/Shader.h>
#include <voxel/VoxelData.h>
#include <voxel/VoxelMeshGenerator.h>
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

	const int gridTileSegments = 4;
	const float gridY = -0.8f;
	std::vector<float> tileVertices, tileColors;
	std::vector<GLuint> tileIndices;
	buildGridTile(gridTileSegments, gridY, tileVertices, tileColors, tileIndices);

	Shader shader;
	shader.LoadFromFiles("../../assets/shaders/default.vert", "../../assets/shaders/default.frag");

	VoxelData voxelData(1, 1, 1);
	voxelData.SetType(0, 0, 0, VoxelType::Stone);
	Mesh cubeMesh;
	VoxelMeshGenerator voxelgenerator;
	voxelgenerator.GenerateMesh(voxelData, cubeMesh);

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
	while (!window.ShouldClose()) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		window.PollEvents();
		camera.Update(&input, deltaTime);

		renderer.Clear();
		for (int tz = 0; tz < gridTilesPerAxis; ++tz){
			for (int tx = 0; tx < gridTilesPerAxis; ++tx) {
				glm::mat4 tileModel = glm::translate(glm::mat4(1.f),
					glm::vec3(tx - gridHalf, 0.f, tz - gridHalf));
				renderer.DrawLines(shader, gridTileMesh, camera.GetViewMatrix(),
					camera.GetProjectionMatrix(), tileModel);
			}
		}


		glm::mat4 voxelModel = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
		renderer.Draw(shader, cubeMesh, camera.GetViewMatrix(),
			camera.GetProjectionMatrix(), voxelModel);
		window.SwapBuffer();
	}
	return 0;
}