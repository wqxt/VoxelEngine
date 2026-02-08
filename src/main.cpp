#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <core/Raycast.h>
#include <render/Render.h>
#include <render/Mesh.h>
#include <render/Shader.h>
#include <voxel/VoxelData.h>
#include <voxel/VoxelMeshGenerator.h>
#include <chrono>
#include <vector>
#include <voxel/Chunk.h>
#include <voxel/ChunkController.h>
#include <debug/Crosshair.h>
#include <debug/TargetOutline.h>



int main() {

	Window window(1600, 720, "VoxelEngine");
	GLFWwindow* glfwWindow = const_cast<GLFWwindow*>(window.GetGLFWWindow());

	Render renderer;
	renderer.Init();



	Shader shader;
	shader.LoadFromFiles("../../assets/shaders/default.vert", "../../assets/shaders/default.frag");

	const int worldSizeX = 5;
	const int worldSizeY = 2;
	const int worldSizeZ = 5;
	VoxelData voxelData(worldSizeX, worldSizeY, worldSizeZ);
	for (int z = 0; z < worldSizeZ; ++z)
		for (int y = 0; y < worldSizeY; ++y)
			for (int x = 0; x < worldSizeX; ++x)
				voxelData.SetType(x, y, z, (y == 0) ? VoxelType::Stone : VoxelType::Grass);

	Mesh worldMesh;
	VoxelMeshGenerator::GenerateMesh(voxelData, worldMesh);

	ChunkController chunkController;
	chunkController.Init(3, 1, 3);

	Input input;
	input.Init(glfwWindow);
	window.PollEvents();

	Camera camera;
	camera.Init();

	Crosshair crosshair;
	crosshair.Init();

	TargetOutline targetOutline;
	targetOutline.Init();

	const float raycastMaxDist = 10.f;
	bool prevLmb = false;
	auto lastTime = std::chrono::high_resolution_clock::now();
	while (!window.ShouldClose()) {

		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;

		window.PollEvents();
		camera.Update(&input, deltaTime);

		auto isSolid = [&voxelData, worldSizeX, worldSizeY, worldSizeZ](int x, int y, int z) {
			if (x < 0 || x >= worldSizeX || y < 0 || y >= worldSizeY || z < 0 || z >= worldSizeZ)
				return false;
			return voxelData.GetType(x, y, z) != VoxelType::Air;
		};
		RaycastHit hit = Raycast::Cast(camera.GetPosition(), camera.GetFront(), raycastMaxDist, isSolid);

		bool lmbNow = input.IsMouseKeyPressed(GLFW_MOUSE_BUTTON_LEFT);
		if (lmbNow && !prevLmb && hit.hit) {
			voxelData.SetType(hit.blockPos.x, hit.blockPos.y, hit.blockPos.z, VoxelType::Air);
			VoxelMeshGenerator::GenerateMesh(voxelData, worldMesh);
		}
		prevLmb = lmbNow;

		renderer.Clear();
		if (hit.hit) {
			float ox = static_cast<float>(hit.blockPos.x) + 0.5f;
			float oy = static_cast<float>(hit.blockPos.y) + 0.5f;
			float oz = static_cast<float>(hit.blockPos.z) + 0.5f;
			targetOutline.SetTransform(ox, oy, oz);
			targetOutline.Draw(camera, renderer, shader);
		}
		crosshair.Draw(camera, renderer, shader);
		glm::mat4 worldModel = glm::mat4(1.f);
		renderer.Draw(shader, worldMesh, camera.GetViewMatrix(), camera.GetProjectionMatrix(), worldModel);
		window.SwapBuffer();
	}
	return 0;
}