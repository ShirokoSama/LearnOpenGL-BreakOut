//
// Created by Srf on 2017/10/5.
//

#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameLevel.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const GLfloat BALL_RADIUS(12.5f);
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

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
    std::vector<GameLevel> Levels;
    GLuint LevelIndex;
    Game(GLuint width, GLuint height);
    ~Game();
    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();
};

#endif //BREAKOUT_GAME_H
