//
// Created by dell on 2017/11/20.
//

#include <iostream>
#include "PostProcessor.h"

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height):
    PostProcessShader(shader), Texture(), Width(width), Height(height),
    Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE) {
    glGenFramebuffers(1, &this->FBO);
    glGenFramebuffers(1, &this->MSFBO);
    glGenRenderbuffers(1, &this->RBO);

    // 绑定至MSFBO与一个RBO，将这个RBO作为MSFBO的COLOR_ATTACHMENT
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
    // 将一个渲染缓冲作为帧缓冲的颜色缓冲附件
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

    // 绑定至FBO
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    // 生成一个纹理，只在内存中预留位置但不传纹理数据
    this->Texture.generate(width, height, nullptr);
    // 将一个纹理作为帧缓冲的颜色缓冲附件，最后一个参数0是多级渐远纹理的level
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    // 绑定回默认帧缓冲
    glBindRenderbuffer(GL_FRAMEBUFFER, 0);

    this->initRenderData();
    this->PostProcessShader.setInteger("scene", 0, GL_TRUE);
    // 这里有些硬编码，因为在着色器里纹理的坐标被标准化至0-1，所以这里用当前设置的屏幕大小预估了采样的偏移
    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
            {-offset, offset},  // top-left
            {0.0f, offset},     // top-center
            {offset, offset },  // top-right
            {-offset, 0.0f},    // center-left
            {0.0f, 0.0f},       // center-center
            {offset, 0.0f},     // center - right
            {-offset, -offset}, // bottom-left
            {0.0f, -offset},    // bottom-center
            {offset, -offset}   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessShader.ID, "offset"), 9, (GLfloat*) offsets);
    // 边缘检测卷积核
    GLint edge_kernel[9] = {
            -1, -1, -1,
            -1, 8, -1,
            -1, -1, -1
    };
    // 模糊卷积核
    glUniform1iv(glGetUniformLocation(this->PostProcessShader.ID, "edge_kernel"), 9, edge_kernel);
    GLfloat blur_kernel[9] = {
            1.0f / 16, 2.0f / 16, 1.0f / 16,
            2.0f / 16, 4.0f / 16, 2.0f / 16,
            1.0f / 16, 2.0f / 16, 1.0f / 16
    };
    glUniform1fv(glGetUniformLocation(this->PostProcessShader.ID, "blur_kernel"), 9 ,blur_kernel);
}

void PostProcessor::BeginRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    // blit（传输？），前四个参数为source，之后四个为destination，由之前的GL_READ_FB到GL_DRAW_FB
    glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // 绑定回默认帧缓冲，此时FBO帧缓冲上作为颜色附件的纹理，已经是正常渲染后的场景了
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time) {
    this->PostProcessShader.use();
    this->PostProcessShader.setFloat("time", time);
    this->PostProcessShader.setInteger("confuse", this->Confuse);
    this->PostProcessShader.setInteger("shake", this->Shake);
    this->PostProcessShader.setInteger("chaos", this->Chaos);
    glActiveTexture(GL_TEXTURE0);
    this->Texture.bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
            // Pos        // Tex
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(this->VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*) nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}