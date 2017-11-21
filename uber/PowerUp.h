//
// Created by dell on 2017/11/21.
//

#ifndef BREAKOUT_POWERUP_H
#define BREAKOUT_POWERUP_H


#include "GameObject.h"

const glm::vec2 POWER_UP_SIZE(60, 20);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp: public GameObject{
public:
    std::string Type;
    GLfloat Duration;
    GLboolean Activated;
    PowerUp(std::string&& type, glm::vec3 color, GLfloat duration,
            glm::vec2 position, Texture2D texture) :
            GameObject(position, POWER_UP_SIZE, texture, color, VELOCITY),
              Type(std::move(type)), Duration(duration), Activated(GL_FALSE) {}
};


#endif //BREAKOUT_POWERUP_H
