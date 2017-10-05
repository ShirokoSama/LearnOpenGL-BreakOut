//
// Created by Srf on 2017/10/5.
//

#include "Game.h"

Game::Game(GLuint width, GLuint height): state(GAME_ACTIVE), keys(), width(width), height(height) {}

Game::~Game() = default;

void Game::init() {}

void Game::processInput(GLfloat dt) {}

void Game::update(GLfloat dt) {}

void Game::render() {}

