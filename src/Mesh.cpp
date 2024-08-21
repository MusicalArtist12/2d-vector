#include "Mesh.h"

#include <cmath>
#include <iostream>

Mesh::Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i): vertices(v), index(i), generated(false) {}

float Mesh::radius() {
    float radius = 0.0f;

    // adjust each point to match the center, then find the distance
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].pos[0] = vertices[i].pos[0] - center().x;
        vertices[i].pos[1] = vertices[i].pos[1] - center().y;
    
        float distance = sqrt(pow(vertices[i].pos[0], 2) + pow((vertices[i].pos[1]), 2));

        if (radius < distance) {
            radius = distance;
        }
    }

    return radius;
}

glm::vec3 Mesh::center() {
    // find the center via the mean
    float x_mean = 0.0f;
    float y_mean = 0.0f;

    for (int i = 0; i < vertices.size(); i++) {
        x_mean += vertices[i].pos[0];
        y_mean += vertices[i].pos[1];
    }  

    x_mean = x_mean / vertices.size();
    y_mean = y_mean / vertices.size();

    return glm::vec3(x_mean, y_mean, 0.0f);
}

Mesh genPolygon(int numSides) {
    if (numSides < 3) {
        throw std::invalid_argument("genPolygon() cannot generate a polygon with less than 3 sides");
    }

    std::vector<Vertex> vertices = {
        Vertex(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f)
    };

    std::vector<unsigned int> index;

    float angle = 360.0/(float)numSides;
    int idx_a = 1; // 1 -> numSides
    int idx_b = 2; // 2 -> numSides -> 1

    for (int i = 0; i < numSides; i++) {
        // essentially, there will be numsides amount of triangles all around the center

        float x = glm::cos(glm::radians(i * angle));
        float y = glm::sin(glm::radians(i * angle));

        vertices.push_back(Vertex(x, y, 1.0f, 1.0f, 1.0f, 1.0f));
        index.push_back(0); 
        
        index.push_back(idx_a);
        idx_a++;

        index.push_back(idx_b);
        idx_b = (idx_b + 1) % (numSides + 1);

        if (idx_b == 0) {
            idx_b = 1;
        }

    }
    
    return Mesh(vertices, index);
}

void setColor(Mesh* mesh, float r, float g, float b, float a) {
    for (int i = 0; i < mesh->vertices.size(); i++) {
        mesh->vertices[i].rgba[0] = r;
        mesh->vertices[i].rgba[1] = g;
        mesh->vertices[i].rgba[2] = b;
        mesh->vertices[i].rgba[3] = a;
    }

    mesh->generated = false;
}