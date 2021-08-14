#pragma once

class particle{
    float speed_x;
    float speed_y;
    int timer;

}

class material{
    vector color;//this will probably get changed from vector
    short constant_force
}

class sand:material{
    constant_force = 10;
    color = {237, 205, 88};
};