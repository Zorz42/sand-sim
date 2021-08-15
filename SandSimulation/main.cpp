#include "particleRenderer.hpp"
#include "particleContainer.hpp"

#define WIDTH 1294
#define HEIGHT 800

int main() {
    particle_container container(WIDTH, HEIGHT);
    ParticleRenderer renderer(&container, WIDTH, HEIGHT);
    while(renderer.isRunning()) {
        renderer.render();
    }
    return 0;
}
