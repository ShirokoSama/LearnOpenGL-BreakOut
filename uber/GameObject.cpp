//
// Created by dell on 2017/11/13.
//

#include "GameObject.h"

GameObject::GameObject()
        : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Texture(), IsSolid(GL_FALSE), Destroyed(GL_FALSE) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D texture2D, glm::vec3 color, glm::vec2 velocity)
        : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Texture(texture2D), IsSolid(GL_FALSE), Destroyed(GL_FALSE) {}

void GameObject::draw(SpriteRenderer &renderer) {
    renderer.drawSprite(this->Texture, this->Position, this->Size, this->Rotation, this->Color);
}