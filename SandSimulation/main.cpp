#include "particleRenderer.hpp"
#include "particleEngine.hpp"
#include "shaders.hpp"

#define WIDTH 800
#define HEIGHT 500

int main() {
    ParticleContainer container(WIDTH, HEIGHT);
    ParticleRenderer renderer(&container, WIDTH, HEIGHT);
    loadShader();

    while(renderer.isRunning()) {
        if(!renderer.isPaused())
            container.updateAll();
        renderer.render();
    }
    return 0;
}
