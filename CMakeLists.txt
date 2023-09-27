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

add_library(gfxEngine
    src/gfxEngine/Camera.cpp 
    src/gfxEngine/RenderEngine.cpp 
    src/gfxEngine/WorldEngine.cpp
)

target_include_directories(gfxEngine
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/glad/include/
    ${PROJECT_SOURCE_DIR}/external/glfw/include/
    ${PROJECT_SOURCE_DIR}/external/glm/include/
    ${PROJECT_SOURCE_DIR}/external/imgui/include/
)

add_library(physicsEngine
    src/physicsEngine/PhysObject.cpp
    src/physicsEngine/PhysicsEngine.cpp
)

target_include_directories(physicsEngine
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/glad/include/
    ${PROJECT_SOURCE_DIR}/external/glfw/include/
    ${PROJECT_SOURCE_DIR}/external/glm/include/
    ${PROJECT_SOURCE_DIR}/external/imgui/include/
)

add_library(glCore
    src/glCore/Shader.cpp 
    src/glCore/WindowEngine.cpp 
)

target_include_directories(glCore
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/glad/include/
    ${PROJECT_SOURCE_DIR}/external/glfw/include/
    ${PROJECT_SOURCE_DIR}/external/glm/include/
    ${PROJECT_SOURCE_DIR}/external/imgui/include/
)

target_link_libraries(gfxEngine glad glfw glm)
target_link_libraries(glCore glad glfw glm)
target_link_libraries(physicsEngine glm)

add_executable(a.out src/main.cpp)

target_include_directories(a.out
PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
    ${PROJECT_SOURCE_DIR}/external/glad/include/
    ${PROJECT_SOURCE_DIR}/external/glfw/include/
    ${PROJECT_SOURCE_DIR}/external/glm/include/
    ${PROJECT_SOURCE_DIR}/external/imgui/include/
)

target_link_libraries(a.out gfxEngine glCore physicsEngine imgui)




