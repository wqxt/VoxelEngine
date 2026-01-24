#include <glad/glad.h>  
#include <iostream>
#include <core/Window.h>
#include <render/Renderer.h>
#include <chrono>

int main() {
    Window window(1600, 720, "VoxelEngine");
    window.PollEvents();

    Renderer renderer;
    renderer.Init();

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        window.PollEvents();

        renderer.Update(deltaTime);
        renderer.Clear();
        renderer.DrawCube();
        window.SwapBuffer();
    }

    return 0;
}