//
// Created by Srf on 2017/10/5.
//

#include "Shader.h"
#include <iostream>

Shader& Shader::use() {
    glUseProgram(this->ID);
    return *this;
}

void Shader::compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource) {
    GLuint sVertex, sFragment, gShader = 0;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // If geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->ID, gShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

void Shader::setFloat(const GLchar *name, GLfloat value, bool useShader) {
    if(useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::setInteger(const GLchar *name, GLint value, bool useShader) {
    if(useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, bool useShader) {
    if(useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, bool useShader) {
    if(useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, bool useShader) {
    if(useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, bool useShader) {
    if(useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, bool useShader) {
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, bool useShader) {
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, bool useShader) {
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}
