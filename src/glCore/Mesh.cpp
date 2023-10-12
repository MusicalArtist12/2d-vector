#include "glCore/Mesh.h"

#include <iostream>
#include <cmath>


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