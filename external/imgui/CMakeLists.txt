project(imgui)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../glfw/include/)


add_library(imgui 
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
    imgui_draw.cpp
    imgui_tables.cpp 
    imgui_widgets.cpp 
    imgui.cpp
)
    
