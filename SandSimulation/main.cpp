#include "renderer.hpp"

#define WIDTH 800
#define HEIGHT 600

int main() {
    Renderer renderer(WIDTH, HEIGHT);
    while(renderer.isRunning())
        renderer.render();
    return 0;
}
