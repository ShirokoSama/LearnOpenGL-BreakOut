//
// Created by Srf on 2017/11/17.
//

#ifndef BREAKOUT_BALLOBJECT_H
#define BREAKOUT_BALLOBJECT_H


#include "GameObject.h"

class BallObject: public GameObject{
public:
    GLfloat Radius;
    GLboolean Stuck;
    GLboolean Sticky, PassThrough;
    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture);
    glm::vec2 Move(GLfloat dt, GLuint windows_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif //BREAKOUT_BALLOBJECT_H
