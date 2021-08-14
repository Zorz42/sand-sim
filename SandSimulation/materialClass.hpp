#pragma once

#include <vector>

struct particle {
    float speed_x;
    float speed_y;
    int timer;
};

struct material {
    std::vector<unsigned char> color;//this will probably get changed from vector
    short constant_force;
};

class sand:material {
public:
    sand() {
        constant_force = 10;
        color = {237, 205, 88};
    }
    void update();
};
