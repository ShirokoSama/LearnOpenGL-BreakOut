//
// Created by Srf on 2017/11/19.
//

#include "ParticleGenerator.h"
#include <random>
#include <iostream>
#include <ctime>

using namespace std;

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount):
    shader(shader), texture(texture), amount(amount){
    this->init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset) {
    for (int i = 0; i < newParticles; i++) {
        int unusedParticleIndex = firstUnusedParticle();
        respawnParticle(this->particles[unusedParticleIndex], object, offset);
    }
    for (Particle &particle: particles) {
        particle.Life -= dt;
        if (particle.Life > 0.0f) {
            particle.Position += particle.Velocity * dt;
            particle.Color -= 2.5f * dt;
        }
    }
}

void ParticleGenerator::Draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader.use();
    for(Particle particle: particles) {
        this->shader.setVector2f("offset", particle.Position);
        this->shader.setVector4f("color", particle.Color);
        this->texture.bind();
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
    GLuint VBO;
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f, //2维顶点坐标+2维纹理坐标
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) nullptr);
    glBindVertexArray(0);

    for(int i = 0; i < this->amount; i++) {
        this->particles.emplace_back(Particle());
    }
}

// Stores the index of the last particle used (for quick access to next dead particle)
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle() {
    for (GLuint i = lastUsedParticle; i < amount; i++) {
        if (this->particles[i].Life < 0) {
            lastUsedParticle = i;
            return i;
        }
    }
    for (GLuint i = 0; i < lastUsedParticle; i++) {
        if (this->particles[i].Life < 0) {
            lastUsedParticle = i;
            return i;
        }
    }
    lastUsedParticle = 0;
    return 0;
}

static default_random_engine e((unsigned int)time(0));
static uniform_real_distribution<float> u(0.0f, 5.0f);
static uniform_real_distribution<float> uc(0.5f, 1.5f);
void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset) {
    GLfloat random = u(e);
    GLfloat rColor = uc(e);
    particle.Position = object.Position + random + offset; //初始位置为小球位置 + 随机偏移 + 偏移量
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * -0.1f; //粒子速度与小球速度相关，方向相反（表现为拖尾）
}




