//
// Created by Srf on 2017/10/5.
//

#include "Game.h"
#include "../util/ResourceManager.h"
#include "../util/SpriteRenderer.h"

SpriteRenderer *renderer;

Game::Game(GLuint width, GLuint height): state(GAME_ACTIVE), keys(), width(width), height(height) {}

Game::~Game() {
    delete renderer;
};

void Game::init() {
    // 加载着色器
    ResourceManager::loadShader("../shader/sprite_vs.glsl", "../shader/sprite_fs.glsl", nullptr, "sprite");
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width),
                                      static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    // 加载纹理
    ResourceManager::loadTexture("../resource/awesomeface.png", GL_TRUE, "face");
    // 设置专用于渲染的控制
    Shader shader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(shader);
}

void Game::processInput(GLfloat dt) {}

void Game::update(GLfloat dt) {}

void Game::render() {
    Texture2D texture = ResourceManager::getTexture("face");
    renderer->drawSprite(texture, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

