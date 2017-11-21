//
// Created by Srf on 2017/11/17.
//

#include "BallObject.h"

BallObject::BallObject(): GameObject(), Radius(12.5f), Stuck(GL_TRUE), Sticky(GL_FALSE), PassThrough(GL_FALSE) {}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture):
    GameObject(pos, glm::vec2(radius * 2, radius * 2), texture, glm::vec3(1.0f), velocity), Radius(radius), Stuck(GL_TRUE),
    Sticky(GL_FALSE), PassThrough(GL_FALSE) {}

glm::vec2 BallObject::Move(GLfloat dt, GLuint windows_width) {
    if (!this->Stuck) {
        this->Position += this->Velocity * dt;
        // x轴边界
        if (this->Position.x < 0) {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0;
        }
        else if (this->Position.x > windows_width - this->Size.x) {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = windows_width - this->Size.x;
        }
        // y轴边界
        if (this->Position.y <0) {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0;
        }
    }
    return this->Position;
}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = GL_TRUE;
}