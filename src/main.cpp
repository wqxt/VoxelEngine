#include <iostream>
#include <core/Window.h>

int main(){

    Window* window = new Window(1920,720, "Engine");


    while (window->ShouldClose() == false)
    {
        window->PollEvents();
        window->SwapBuffer();
    }

    delete window;
    return 0;
}