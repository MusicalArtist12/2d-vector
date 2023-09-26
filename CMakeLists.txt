cmake_minimum_required(VERSION 3.25.1)

project(minesweeper_openGL VERSION 0.1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)

set(INCLUDES
"src/main.cpp"
"glad/src/gl.c"
)
add_executable(a.out ${INCLUDES} )
set_target_properties(a.out PROPERTIES RUNTIME_OUTPUT_DIRECTORY "binary")

target_include_directories(a.out
    PUBLIC glfw/include
    PUBLIC glad/include
    PUBLIC glm/
    PUBILC imgui/include
)

target_link_directories(a.out
    PRIVATE glfw/src
    PRIVATE glad/src 
    PRIVATE imgui
)

target_link_libraries(a.out 
    glfw
    imgui
    glm
)


