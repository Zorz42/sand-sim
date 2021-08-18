#include "particleRenderer.hpp"
#include "particleEngine.hpp"

#define WIDTH 800
#define HEIGHT 500

int main() {
    ParticleContainer container(WIDTH, HEIGHT);
    ParticleRenderer renderer(&container, WIDTH, HEIGHT);

    while(renderer.isRunning()) {
        container.updateAll();
        renderer.render();
    }
    return 0;
}
