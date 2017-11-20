//
// Created by Srf on 2017/10/5.
//

#include "Game.h"
#include "../util/ResourceManager.h"
#include "BallObject.h"
#include "ParticleGenerator.h"

SpriteRenderer *renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;

Game::Game(GLuint width, GLuint height): state(GAME_ACTIVE), keys(), width(width), height(height) {}

Game::~Game() {
    delete renderer;
    delete Player;
    delete Ball;
};

void Game::init() {
    // 加载着色器
    ResourceManager::loadShader("../shader/sprite_vs.glsl", "../shader/sprite_fs.glsl", nullptr, "sprite");
    ResourceManager::loadShader("../shader/particle_vs.glsl", "../shader/particle_fs.glsl", nullptr, "particle");

    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width),
                                      static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    ResourceManager::getShader("particle").use().setInteger("sprite", 0);
    ResourceManager::getShader("particle").setMatrix4("projection", projection);

    // Load textures
    ResourceManager::loadTexture("../resource/background.jpg", GL_FALSE, "background");
    ResourceManager::loadTexture("../resource/awesomeface.png", GL_TRUE, "face");
    ResourceManager::loadTexture("../resource/block.png", GL_FALSE, "block");
    ResourceManager::loadTexture("../resource/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::loadTexture("../resource/paddle.png", GL_TRUE, "paddle");
    ResourceManager::loadTexture("../resource/particle.png", GL_TRUE, "particle");

    // 设置专用于渲染的控制
    Shader shader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(shader);
    Particles = new ParticleGenerator(
            ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);

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

    // 挡板
    glm::vec2 playerPos(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

    // 小球
    glm::vec2 ballPos = playerPos + glm::vec2(Player->Size.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));
}

void Game::processInput(GLfloat dt) {
    if (this->state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        if (this->keys[GLFW_KEY_A]) {
            Player->Position.x -= velocity;
            if (Player->Position.x < 0)
                Player->Position.x = 0;
            if (Ball->Stuck)
                Ball->Position.x = Player->Position.x + Player->Size.x / 2 - BALL_RADIUS;
        }
        if (this->keys[GLFW_KEY_D]) {
            Player->Position.x += velocity;
            if (Player->Position.x > this -> width - Player->Size.x)
                Player->Position.x = this -> width - Player->Size.x;
            if (Ball->Stuck)
                Ball->Position.x = Player->Position.x + Player->Size.x / 2 - BALL_RADIUS;
        }
        if (this->keys[GLFW_KEY_SPACE])
            Ball->Stuck = GL_FALSE;
        if (this->keys[GLFW_KEY_R]) {
            this->ResetLevel();
            this->ResetPlayer();
        }
    }
}

void Game::update(GLfloat dt) {
    Ball->Move(dt, this->width);
    this->DoCollision();
    Particles->Update(dt, *Ball, 5, glm::vec2(Ball->Radius / 2));
    if (Ball->Position.y >= this->height) {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::render() {
    if(this->state == GAME_ACTIVE) {
        // Draw background
        Texture2D texture = ResourceManager::getTexture("background");
        renderer->drawSprite(texture, glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
        // Draw level
        this->Levels[this->LevelIndex].Draw(*renderer);
        Player->draw(*renderer);
        Particles->Draw();
        Ball->draw(*renderer);
    }
}

void Game::ResetLevel() {
    if (this->LevelIndex == 0)
        this->Levels[0].Load("../levels/one.lvl", this->width, this->height / 2);
    else if (this->LevelIndex == 1)
        this->Levels[1].Load("../levels/two.lvl", this->width, this->height / 2);
    else if (this->LevelIndex == 2)
        this->Levels[2].Load("../levels/three.lvl", this->width, this->height / 2);
    else if (this->LevelIndex == 3)
        this->Levels[3].Load("../levels/four.lvl", this->width, this->height/ 2);
}

void Game::ResetPlayer() {
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

bool CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

void Game::DoCollision() {
    for (GameObject &obj: this->Levels[this->LevelIndex].Bricks) {
        if (!obj.Destroyed) {
            Collision collision = CheckCollision(*Ball, obj);
            if (std::get<0>(collision)) {
                if (!obj.IsSolid)
                    obj.Destroyed = GL_TRUE;
                Direction dir = std::get<1>(collision);
                glm::vec2 offset = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) {
                    Ball->Velocity.x = -Ball->Velocity.x;
                    GLfloat movement = std::abs(offset.x);
                    if (dir == LEFT)
                        Ball->Position.x -= movement;
                    else
                        Ball->Position.x += movement;
                }
                else {
                    Ball->Velocity.y = -Ball->Velocity.y;
                    GLfloat movement = std::abs(offset.y);
                    if (dir == UP)
                        Ball->Position.y -= movement;
                    else
                        Ball->Position.y += movement;
                }
            }
        }
    }
    Collision player_collision = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(player_collision)) {
        GLfloat player_center = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = Ball->Position.x + Ball->Radius - player_center;
        GLfloat percentage = distance / (Player->Size.x / 2);
        GLfloat strength = 2.0f;
        GLfloat velocity_length = glm::length(Ball->Velocity);
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity.y = -std::abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * velocity_length;
    }
}

bool CheckCollision(GameObject &one, GameObject &two) {
    bool x_collision = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
    bool y_collision = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
    return x_collision || y_collision;
}

Collision CheckCollision(BallObject &one, GameObject &two) {
    glm::vec2 half_aabb(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 ball_center(one.Position + one.Radius);
    glm::vec2 aabb_center(two.Position + half_aabb);
    glm::vec2 difference = ball_center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -half_aabb, half_aabb);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - ball_center;
    return glm::length(difference) < one.Radius ? std::make_tuple(GL_TRUE, VectorDirection(difference), difference)
                                                : std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction VectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),	// up
            glm::vec2(1.0f, 0.0f),	// right
            glm::vec2(0.0f, -1.0f),	// down
            glm::vec2(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;
    GLint match = -1;
    // 最近点与圆心的向量点乘四个方向的向量的最大值为碰撞方向
    for (int i = 0; i < 4; i++) {
        float cos = glm::dot(glm::normalize(target), compass[i]);
        if (cos > max) {
            max = cos;
            match = i;
        }
    }
    return (Direction)match;
}

