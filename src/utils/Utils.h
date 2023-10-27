#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <glm>

template <typename T>
inline std::string toString(const T a_value, const int n = 6) {
    return std::to_string(a_value).substr(0, std::to_string(a_value).find(".") + n + 1);
}

float length(glm::vec3 val) {
    float val = pow(val.x,2) + pow(val.y,2);
    return sqrt(val);
}

#endif
