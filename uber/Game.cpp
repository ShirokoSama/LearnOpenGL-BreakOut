//
// Created by Srf on 2017/10/5.
//

#include "Game.h"
#include "../util/ResourceManager.h"

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

SpriteRenderer *renderer;

GameObject *Player;

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
    // 设置专用于渲染的控制
    Shader shader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(shader);

    // Load textures
    ResourceManager::loadTexture("../resource/background.jpg", GL_FALSE, "background");
    ResourceManager::loadTexture("../resource/awesomeface.png", GL_TRUE, "face");
    ResourceManager::loadTexture("../resource/block.png", GL_FALSE, "block");
    ResourceManager::loadTexture("../resource/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::loadTexture("../resource/paddle.png", GL_TRUE, "paddle");

    // Load levels
    GameLevel one;
    one.Load("../levels/one.lvl", this->width, this->height / 2);
    GameLevel two;
    two.Load("../levels/two.lvl", this->width, this->height / 2);
    GameLevel three;
    three.Load("../levels/three.lvl", this->width, this->height / 2);
    GameLevel four;
    four.Load("../levels/four.lvl", this->width, this->height / 2);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->LevelIndex = 0;

    glm::vec2 playerPos(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));
}

void Game::processInput(GLfloat dt) {
    if (this->state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        if (this->keys[GLFW_KEY_A]) {
            if (Player->Position.x >= 0) {
                float tmpPos = Player->Position.x - velocity;
                Player->Position.x = tmpPos >= 0 ? tmpPos : 0;
            }
        }
        if (this->keys[GLFW_KEY_D]) {
            if (Player->Position.x <= this -> width - Player->Size.x) {
                float tmpPos = Player->Position.x + velocity;
                Player->Position.x = tmpPos <= this->width - Player->Size.x ? tmpPos : this->width - Player->Size.x;
            }
        }
    }
}

void Game::update(GLfloat dt) {}

void Game::render() {
    if(this->state == GAME_ACTIVE) {
        // Draw background
        Texture2D texture = ResourceManager::getTexture("background");
        renderer->drawSprite(texture, glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
        // Draw level
        this->Levels[this->LevelIndex].Draw(*renderer);
        Player->draw(*renderer);
    }
}

