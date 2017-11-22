//
// Created by Srf on 2017/11/22.
//

#ifndef BREAKOUT_TEXTRENDERER_H
#define BREAKOUT_TEXTRENDERER_H

#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "ResourceManager.h"

struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class TextRenderer {
public:
    std::map<GLchar, Character> Characters;
    Shader TextShader;
    TextRenderer(GLuint width, GLuint height);
    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

private:
    GLuint VAO, VBO;
};


#endif //BREAKOUT_TEXTRENDERER_H
