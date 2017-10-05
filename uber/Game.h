//
// Created by Srf on 2017/10/5.
//

#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public:
    GameState state;
    GLboolean keys[1024];
    GLuint width, height;
    Game(GLuint width, GLuint height);
    ~Game();
    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();
};


#endif //BREAKOUT_GAME_H
