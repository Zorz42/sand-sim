#include "particleRenderer.hpp"
#include "particleContainer.hpp"

#define WIDTH 800
#define HEIGHT 500

int main() {
    initMaterials();
    ParticleContainer container(WIDTH, HEIGHT);
    ParticleRenderer renderer(&container, WIDTH, HEIGHT);

    while(renderer.isRunning()) {
        container.updateAll();
        renderer.render();
    }
    return 0;
}
