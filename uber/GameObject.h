//
// Created by dell on 2017/11/13.
//

#ifndef BREAKOUT_GAMEOBJECT_H
#define BREAKOUT_GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../util/Texture2D.h"
#include "../util/SpriteRenderer.h"

class GameObject {
public:
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    GLfloat Rotation;
    GLboolean IsSolid;
    GLboolean Destroyed;
    Texture2D Texture;
    // Render State
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture2D, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void draw(SpriteRenderer &renderer);
};


#endif //BREAKOUT_GAMEOBJECT_H
