#include "particleRenderer.hpp"
#include "particleContainer.hpp"

#define WIDTH 1618
#define HEIGHT 1000

int main() {
    particle_container container(WIDTH, HEIGHT);
    ParticleRenderer renderer(&container, WIDTH, HEIGHT);
    while(renderer.isRunning()) {
        renderer.render();
    }
    return 0;
}
