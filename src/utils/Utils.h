#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <glm/glm.hpp>

template <typename T>
inline std::string toString(const T a_value, const int n = 6) {
    return std::to_string(a_value).substr(0, std::to_string(a_value).find(".") + n + 1);
}

#endif
