#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
};

void shader::bind() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glUseProgram(ID);
}

void shader::unbind() {
    glUseProgram(0);
}

void shader::setUniform3fv(const std::string& name, glm::mat3 matrix) {
    bind();

    int matrixID = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix3fv(matrixID, 1, GL_FALSE, &matrix[0][0]);

    if(matrixID == -1) std::cout << name << " cannot be found" << std::endl;

}

void shader::setUniform4fv(const std::string& name, glm::mat4 matrix) {
    bind();

    int matrixID = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &matrix[0][0]);

    if(matrixID == -1) std::cout << name << " cannot be found" << std::endl;

}

void shader::setUniform1i(const std::string& name, int val) {
    bind();
    
    int ufID = glGetUniformLocation(ID, name.c_str());
    glUniform1i(ufID, val);

    if(ufID == -1) std::cout << name << " cannot be found" << std::endl;
}

void shader::setUniform1f(const std::string& name, float val) {
    bind();
    
    int ufID = glGetUniformLocation(ID, name.c_str());
    glUniform1f(ufID, val);

    if(ufID == -1) std::cout << name << " cannot be found" << std::endl;
}

void shader::setUniform3f(const std::string& name, glm::vec3 val) {
    bind();
    
    int ufID = glGetUniformLocation(ID, name.c_str());
    glUniform3f(ufID, val.x, val.y, val.z);

    if(ufID == -1) std::cout << name << " cannot be found" << std::endl;
}

void shader::setUniform4f(const std::string& name, glm::vec4 val) {
    bind();
    
    int ufID = glGetUniformLocation(ID, name.c_str());
    glUniform4f(ufID, val.x, val.y, val.z, val.w);

    if(ufID == -1) std::cout << name << " cannot be found" << std::endl;
}

shader::shader(const char* vertex_path, const char* fragment_path) {

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertex_path);
    fShaderFile.open(fragment_path);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    std::string vertexCode = vShaderStream.str();
    std::string fragmentCode = fShaderStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    // print compile errors if any
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID); 

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        std::cin.ignore();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}
