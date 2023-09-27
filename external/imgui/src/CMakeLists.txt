include_directories(${PROJECT_SOURCE_DIR}/include/)

include_directories(${PROJECT_SOURCE_DIR}/../glfw/include/)

add_library(imgui 
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_tables.cpp 
    imgui_widgets.cpp
    imgui.cpp 
)
