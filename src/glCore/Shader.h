#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "glCore/Mesh.h"

class shader {
    public:
        unsigned int ID;

        void unbind();
        void bind();

        void setUniform3fv(const std::string& name, glm::mat3 matrix);
        void setUniform4fv(const std::string& name, glm::mat4 matrix);

        void setUniform1i(const std::string& name, int val);

        void setUniform1f(const std::string& name, float val);
        void setUniform3f(const std::string& name, glm::vec3 val);
        void setUniform4f(const std::string& name, glm::vec4 val);

        shader(const char* vertex_path, const char* fragment_path);

        void drawMesh(GLuint VAO, int size, glm::mat4 model);
        void setView(glm::mat4 view, glm::mat4 projection);

        void generateBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, vertex* vertices, int vSize, GLuint* index, int iSize);
};

#endif