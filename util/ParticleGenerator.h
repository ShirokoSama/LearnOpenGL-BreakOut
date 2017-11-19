//
// Created by Srf on 2017/11/19.
//

#ifndef BREAKOUT_PARTICLEGENERATOR_H
#define BREAKOUT_PARTICLEGENERATOR_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

#include "Texture2D.h"
#include "Shader.h"
#include "../uber/GameObject.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    GLfloat Life;

    Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f));
    void Draw();

private:
    std::vector<Particle> particles;
    GLuint amount;
    GLuint VAO;
    Shader shader;
    Texture2D texture;
    void init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f));
};


#endif //BREAKOUT_PARTICLEGENERATOR_H
