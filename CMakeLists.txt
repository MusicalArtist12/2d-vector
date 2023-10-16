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

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

add_executable(a.out 
    ${SRC_DIR}/Window.cpp
    ${SRC_DIR}/Mesh.cpp
    ${SRC_DIR}/Shader.cpp 
    ${SRC_DIR}/Camera.cpp
    ${SRC_DIR}/Render.cpp 
    ${SRC_DIR}/World.cpp
    ${SRC_DIR}/main.cpp
)

target_link_libraries(a.out glm imgui glad glfw)

target_include_directories(a.out
    PUBLIC
        ${SRC_DIR}/include
        ${SRC_DIR}/
        ${PROJECT_SOURCE_DIR}/external/glm/
        ${PROJECT_SOURCE_DIR}/external/imgui/include/
        ${PROJECT_SOURCE_DIR}/external/glad/include/
        ${PROJECT_SOURCE_DIR}/external/glfw/include/
)