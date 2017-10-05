//
// Created by Srf on 2017/10/5.
//

#ifndef BREAKOUT_SHADER_H
#define BREAKOUT_SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;
    Shader() = default;
    Shader &use();
    void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
    void setFloat(const GLchar *name, GLfloat value, bool useShader = false);
    void setInteger(const GLchar *name, GLint value, bool useShader = false);
    void setVector2f(const GLchar *name, GLfloat x, GLfloat y, bool useShader = false);
    void setVector2f(const GLchar *name, const glm::vec2 &value, bool useShader = false);
    void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, bool useShader = false);
    void setVector3f(const GLchar *name, const glm::vec3 &value, bool useShader = false);
    void setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, bool useShader = false);
    void setVector4f(const GLchar *name, const glm::vec4 &value, bool useShader = false);
    void setMatrix4(const GLchar *name, const glm::mat4 &matrix, bool useShader = false);

private:
    void checkCompileErrors(GLuint object, std::string type);
};


#endif //BREAKOUT_SHADER_H
