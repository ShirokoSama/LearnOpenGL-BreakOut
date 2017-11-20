//
// Created by dell on 2017/11/20.
//

#ifndef BREAKOUT_POSTPROCESSOR_H
#define BREAKOUT_POSTPROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../util/Texture2D.h"
#include "../util/SpriteRenderer.h"
#include "../util/Shader.h"

class PostProcessor {
public:
    Shader PostProcessShader;
    Texture2D Texture;
    GLuint Width, Height;
    GLboolean Confuse, Shake, Chaos;
    PostProcessor(Shader shader, GLuint width, GLuint height);
    void BeginRender();
    void EndRender();
    void Render(GLfloat time);

private:
    GLuint FBO, MSFBO, RBO, VAO;
    void initRenderData();
};


#endif //BREAKOUT_POSTPROCESSOR_H
