#include "Mesh.h"

#include <cmath>
#include <iostream>

mesh::mesh(std::vector<vertex>& v, std::vector<unsigned int>& i)
    : vertices(v), index(i), generated(false) {

    // find the center via the mean
    float x_mean = 0;
    float y_mean = 0;

    for(int i = 0; i < v.size(); i++) {
        x_mean += v[i].pos[0];
        y_mean += v[i].pos[1];
    }  

    x_mean = x_mean / v.size();
    y_mean = y_mean / v.size();


    radius = 0.0;

    // adjust each point to match the center, then find the distance
    for(int i = 0; i < v.size(); i++) {
        v[i].pos[0] = v[i].pos[0] - x_mean;
        v[i].pos[1] = v[i].pos[1] - y_mean;

        float distance = sqrt( pow(v[i].pos[0], 2) + pow((v[i].pos[1]), 2) );

        if(radius < distance) radius = distance;
    }
}

mesh genPolygon(int numSides) {
    if(numSides < 3) throw std::invalid_argument("genPolygon() cannot generate a polygon with less than 3 sides");

    std::vector<vertex> vertices = {
        vertex(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
    };

    std::vector<unsigned int> index;

    float angle = 360.0/(float)numSides;
    int idx_a = 1; // 1 -> numSides
    int idx_b = 2; // 2 -> numSides -> 1

    for(int i = 0; i < numSides; i++) {
        // essentially, there will be numsides amount of triangles all around the center

        float x = cos((i * angle) * 180.0/M_PI);
        float y = sin((i * angle) * 180.0/M_PI);

        vertices.push_back(vertex(x, y, 1.0f, 1.0f, 1.0f, 1.0f));
        index.push_back(0); 
        
        index.push_back(idx_a);
        idx_a++;

        index.push_back(idx_b);
        idx_b = (idx_b + 1) % (numSides + 1);

        if(idx_b == 0) idx_b = 1;

    }
    
    return mesh(vertices, index);
}