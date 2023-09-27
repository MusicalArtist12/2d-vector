#ifndef VERTEX_H
#define VERTEX_H

struct vertex {
    float pos[3];
    float rgba[4];

    vertex() {}

    vertex(float x, float y, float r, float g, float b, float a) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = 0.0;

        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }
};

#endif
