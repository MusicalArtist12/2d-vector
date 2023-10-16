cmake_minimum_required(VERSION 3.25.1)

project(2D_PHYSICS_ENGINE VERSION 0.1)

add_compile_options(-Wall)
set(CMAKE_CXX_STANDARD 11)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/imgui)

