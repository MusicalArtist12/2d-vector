#version 330 core

layout (location = 0) in vec2 aPos; 
layout (location = 1) in vec4 rgba; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;  

out vec4 color;

void main() {
    vec3 model_3d = vec3(aPos, 1.0);

    gl_Position =  projection * view * model * vec4(model_3d, 1.0);

    FragPos = vec3(model * vec4(model_3d, 1.0));

    color = rgba;
}  