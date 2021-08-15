#include "particleRenderer.hpp"
#include "particleContainer.hpp"

#define WIDTH 640
#define HEIGHT 400

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
