#include "renderer.hpp"

int main() {
    Renderer renderer;
    renderer.init();
    while(renderer.isOpen())
        renderer.render();
    return 0;
}
