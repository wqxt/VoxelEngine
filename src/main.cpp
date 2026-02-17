#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <core/Window.h>
#include <core/Camera.h>
#include <core/Input.h>
#include <core/Raycast.h>
#include <render/Render.h>
#include <render/Mesh.h>
#include <render/Shader.h>
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

	const int chunkSizeX = 5;
	const int chunkSizeY = 10;
	const int chunkSizeZ = 5;
	const int gridX = 10;
	const int gridY = 10;
	const int gridZ = 10;

	ChunkController chunkController;
	chunkController.InitGrid(gridX, gridY, gridZ);
	chunkController.SetupChunks(chunkSizeX, chunkSizeY, chunkSizeZ);

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

		RaycastHit hit = Raycast::Cast(camera.GetPosition(), camera.GetFront(), raycastMaxDist, chunkController.GetIsSolidCallback());

		bool lmbNow = input.IsMouseKeyPressed(GLFW_MOUSE_BUTTON_LEFT);
		if (lmbNow && !prevLmb && hit.hit)
		{
			chunkController.DestroyBlock(hit.blockPos.x, hit.blockPos.y, hit.blockPos.z);
		}
		prevLmb = lmbNow;

		renderer.Clear();

		for (const Chunk& chunk : chunkController.GetChunks()) 
		{
			Mesh* mesh = chunk.GetMesh();
			if (!mesh)
				continue;
			const glm::ivec3& pos = chunk.GetPosition();
			glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(
				static_cast<float>(pos.x * chunkController.GetChunkSizeX()),
				static_cast<float>(pos.y * chunkController.GetChunkSizeY()),
				static_cast<float>(pos.z * chunkController.GetChunkSizeZ())));
			renderer.Draw(shader, *mesh, camera.GetViewMatrix(), camera.GetProjectionMatrix(), model);
		}

		if (hit.hit) {
			float ox = static_cast<float>(hit.blockPos.x) + 0.5f;
			float oy = static_cast<float>(hit.blockPos.y) + 0.5f;
			float oz = static_cast<float>(hit.blockPos.z) + 0.5f;
			targetOutline.SetTransform(ox, oy, oz);
			targetOutline.Draw(camera, renderer, shader);
		}
		crosshair.Draw(camera, renderer, shader);
		window.SwapBuffer();
	}
	return 0;
}