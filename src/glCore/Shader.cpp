#include "glCore/Shader.h"

void shader::drawMesh(GLuint VAO, int size, glm::mat4 model) {
    bind();

    setUniform4fv("model", model);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    unbind();
}

void shader::setView(glm::mat4 view, glm::mat4 projection) {
    bind();

    setUniform4fv("view", view);
    setUniform4fv("projection", projection);

    unbind();
}

void shader::generateBuffer(GLuint* VAO, GLuint* VBO, GLuint* EBO, vertex* vertices, int vSize, GLuint* index, int iSize) {
    glGenVertexArrays(1, VAO); 

    glBindVertexArray(*VAO);

        glGenBuffers(1, VBO);
        glGenBuffers(1, EBO);

        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, (vSize * sizeof(vertex)), vertices, GL_STATIC_DRAW);
        // type of buffer, size of buffer we want to pass, data, how its managed
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (iSize * sizeof(GLuint)), index, GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex rgba
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindVertexArray(0); 
}

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
