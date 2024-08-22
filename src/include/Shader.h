#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "Mesh.h"

class BufferMap {
    public:
        GLuint* ID;
        GLenum type;
        GLenum access;

        void* data;

        BufferMap(GLuint* ID, GLenum type, GLenum access);

        ~BufferMap();
};

class Shader {
    private:
        unsigned int ID;

    public:
        void unbind();
        void bind();

        void setUniform3fv(const std::string& name, glm::mat3 matrix);
        void setUniform4fv(const std::string& name, glm::mat4 matrix);

        void setUniform1i(const std::string& name, int val);

        void setUniform1f(const std::string& name, float val);
        void setUniform3f(const std::string& name, glm::vec3 val);
        void setUniform4f(const std::string& name, glm::vec4 val);

        Shader(const char* vertex_path, const char* fragment_path);

        void drawMesh(GLuint VAO, int size, glm::mat4 model);
        void setView(glm::mat4 view, glm::mat4 projection);

        static void generateBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, std::vector<Vertex>& v, std::vector<unsigned int>& i);
};

#endif