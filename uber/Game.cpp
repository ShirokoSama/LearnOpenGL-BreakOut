//
// Created by Srf on 2017/10/5.
//

#include <random>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <irrKlang/irrKlang.h>
#include <sstream>
#include "Game.h"
#include "../util/ResourceManager.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "../util/TextRenderer.h"

using namespace irrklang;

SpriteRenderer *renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;
PostProcessor *Effects;
ISoundEngine *SoundEngine;
TextRenderer *Text;

GLfloat ShakeTime = 0.0f;

Game::Game(GLuint width, GLuint height): state(GAME_ACTIVE), keys(), width(width), height(height) {}

Game::~Game() {
    delete renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
    delete SoundEngine;
    delete Text;
};

void Game::init() {
    // 加载着色器
    ResourceManager::loadShader("../shader/sprite_vs.glsl", "../shader/sprite_fs.glsl", nullptr, "sprite");
    ResourceManager::loadShader("../shader/particle_vs.glsl", "../shader/particle_fs.glsl", nullptr, "particle");
    ResourceManager::loadShader("../shader/post_processing_vs.glsl", "../shader/post_processing_fs.glsl", nullptr, "postprocessing");

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
    ResourceManager::loadTexture("../resource/powerup_speed.png", GL_TRUE, "powerup_speed");
    ResourceManager::loadTexture("../resource/powerup_sticky.png", GL_TRUE, "powerup_sticky");
    ResourceManager::loadTexture("../resource/powerup_increase.png", GL_TRUE, "powerup_increase");
    ResourceManager::loadTexture("../resource/powerup_confuse.png", GL_TRUE, "powerup_confuse");
    ResourceManager::loadTexture("../resource/powerup_chaos.png", GL_TRUE, "powerup_chaos");
    ResourceManager::loadTexture("../resource/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");

    // 设置专用于渲染的控制
    Shader shader = ResourceManager::getShader("sprite");
    renderer = new SpriteRenderer(shader);
    Particles = new ParticleGenerator(
            ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);
    Effects = new PostProcessor(ResourceManager::getShader("postprocessing"), this->width, this->height);

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

    SoundEngine = createIrrKlangDevice();
    SoundEngine->play2D("../resource/breakout.mp3", true);
//    Effects->Chaos = GL_TRUE;
//    Effects->Confuse = GL_TRUE;
//    Effects->Shake = GL_TRUE;
    Text = new TextRenderer(this->width, this->height);
    Text->Load("../resource/OCRAEXT.TTF", 24);
    this->Lives = 3;
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
    if (this->state == GAME_MENU) {
        if (this->keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
            this->state = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
        }
        if (this->keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W]) {
            this->LevelIndex = (this->LevelIndex + 1) % 4;
            this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
        }
        if (this->keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S]) {
            if (this->LevelIndex > 0)
                --this->LevelIndex;
            else
                this->LevelIndex = 3;
            this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
        }
    }
    if (this->state == GAME_WIN) {
        if (this->keys[GLFW_KEY_ENTER]) {
            this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
            Effects->Chaos = GL_FALSE;
            this->state = GAME_MENU;
        }
    }
}

void Game::update(GLfloat dt) {
    if (this->state == GAME_ACTIVE && this->Levels[this->LevelIndex].IsComplete()) {
        this->ResetLevel();
        this->ResetPlayer();
        Effects->Chaos = GL_TRUE;
        this->state = GAME_WIN;
    }
    Ball->Move(dt, this->width);
    this->DoCollision();
    Particles->Update(dt, *Ball, 5, glm::vec2(Ball->Radius / 2));
    if (Ball->Position.y >= this->height) {
        this->Lives--;
        if (this->Lives == 0) {
            this->ResetLevel();
            this->state = GAME_MENU;
        }
        this->ResetPlayer();
    }
    if (ShakeTime > 0.0f) {
        ShakeTime -= dt;
        if (ShakeTime < 0.0f)
            Effects->Shake = GL_FALSE;
    }
    this->UpdatePowerUps(dt);
}

void Game::render() {
    if(this->state == GAME_ACTIVE || this->state == GAME_MENU) {
        Effects->BeginRender();
            // Draw background
            Texture2D texture = ResourceManager::getTexture("background");
            renderer->drawSprite(texture, glm::vec2(0, 0), glm::vec2(this->width, this->height), 0.0f);
            // Draw level
            this->Levels[this->LevelIndex].Draw(*renderer);
            Player->draw(*renderer);
            Particles->Draw();
            Ball->draw(*renderer);
            for (PowerUp &powerUp: PowerUps) {
                if (!powerUp.Destroyed)
                    powerUp.draw(*renderer);
            }
        Effects->EndRender();
        Effects->Render((GLfloat)glfwGetTime());

        std::stringstream ss;
        ss << this->Lives;
        Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
    }
    if (this->state == GAME_MENU)
    {
        Text->RenderText("Press ENTER to start", 250.0f, height / 2, 1.0f);
        Text->RenderText("Press W or S to select level", 245.0f, height / 2 + 20.0f, 0.75f);
    }
    if (this->state == GAME_WIN) {
        Text->RenderText(
                "You WON!!!", 320.0, height / 2 - 20.0f, 1.0, glm::vec3(0.0, 1.0, 0.0)
        );
        Text->RenderText(
                "Press ENTER to retry or ESC to quit", 130.0, height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
        );
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
    this->Lives = 3;
}

void Game::ResetPlayer() {
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->width / 2 - PLAYER_SIZE.x / 2, this->height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


void Game::SpawnPowerUps(GameObject &block) {
    static default_random_engine e((unsigned int)time(nullptr));
    static uniform_int_distribution<int> u(1, 20);
    int random = u(e);
    if (random == 1)
        this->PowerUps.emplace_back(
                PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::getTexture("powerup_speed"))
        );
    if (random == 2)
        this->PowerUps.emplace_back(
                PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::getTexture("powerup_sticky"))
        );
    if (random == 3)
        this->PowerUps.emplace_back(
                PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::getTexture("powerup_passthrough"))
        );
    if (random == 4)
        this->PowerUps.emplace_back(
                PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::getTexture("powerup_increase"))
        );
    if (random == 5)
        this->PowerUps.emplace_back(
                PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::getTexture("powerup_confuse"))
        );
    if (random == 6)
        this->PowerUps.emplace_back(
                PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::getTexture("powerup_chaos"))
        );
}

void ActivatePowerUp(PowerUp &powerUp) {
    if (powerUp.Type == "speed")
        Ball->Velocity *= 1.2;
    else if (powerUp.Type == "sticky") {
        Ball->Sticky = GL_TRUE;
        Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through") {
        Ball->PassThrough = GL_TRUE;
        Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
        Player->Size.x += 50.0f;
    else if (powerUp.Type == "confuse") {
        if (!Effects->Chaos)
            Effects->Confuse = GL_TRUE;
    }
    else if (powerUp.Type == "chaos") {
        if (!Effects->Confuse)
            Effects->Chaos = GL_TRUE;
    }
}

bool isOtherPowerUpActivate(std::list<PowerUp> &powerUps, const std::string &type) {
    for (const PowerUp &powerUp: powerUps) {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return true;
    }
    return false;
}

void Game::UpdatePowerUps(GLfloat dt) {
    for (auto itr = this->PowerUps.begin(); itr != this->PowerUps.end(); itr++) {
        PowerUp &powerUp = *itr;
        powerUp.Position += powerUp.Velocity * dt;
        if (powerUp.Activated) {
            powerUp.Duration -= dt;
            if (powerUp.Duration <= 0.0f) {
                powerUp.Activated = GL_FALSE;
                if (powerUp.Type == "sticky") {
                    if (!isOtherPowerUpActivate(this->PowerUps, powerUp.Type))
                        Ball->Sticky = GL_FALSE;
                }
                else if (powerUp.Type == "pass-through") {
                    if (!isOtherPowerUpActivate(this->PowerUps, powerUp.Type))
                        Ball->PassThrough = GL_FALSE;
                }
                else if (powerUp.Type == "confuse") {
                    if (!isOtherPowerUpActivate(this->PowerUps, powerUp.Type))
                        Effects->Confuse = GL_FALSE;
                }
                else if (powerUp.Type == "chaos") {
                    if (!isOtherPowerUpActivate(this->PowerUps, powerUp.Type))
                        Effects->Chaos = GL_FALSE;
                }
                this->PowerUps.erase(itr);
            }
        }
    }
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp &powerUp) { return !powerUp.Activated && powerUp.Destroyed; }
    ), this->PowerUps.end());
}

bool CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

void Game::DoCollision() {
    for (GameObject* obj_p: this->Levels[this->LevelIndex].CollisionBroadPhase(*Ball)) {
        GameObject& obj = *obj_p;
        if (!obj.Destroyed) {
            Collision collision = CheckCollision(*Ball, obj);
            if (std::get<0>(collision)) {
                if (!obj.IsSolid) {
                    obj.Destroyed = GL_TRUE;
                    this->SpawnPowerUps(obj);
                    SoundEngine->play2D("../resource/bleep.mp3", false);
                }
                else {
                    ShakeTime = 0.05f;
                    Effects->Shake = GL_TRUE;
                    SoundEngine->play2D("../resource/solid.wav", false);
                }
                Direction dir = std::get<1>(collision);
                glm::vec2 offset = std::get<2>(collision);
                if (!(Ball->PassThrough && !obj.IsSolid)) {
                    if (dir == LEFT || dir == RIGHT) {
                        Ball->Velocity.x = -Ball->Velocity.x;
                        GLfloat movement = std::abs(offset.x);
                        GLfloat penetration = Ball->Radius - movement;
                        if (dir == LEFT)
                            Ball->Position.x -= penetration;
                        else
                            Ball->Position.x += penetration;
                    } else {
                        Ball->Velocity.y = -Ball->Velocity.y;
                        GLfloat movement = std::abs(offset.y);
                        GLfloat penetration = Ball->Radius - movement;
                        if (dir == UP)
                            Ball->Position.y -= penetration;
                        else
                            Ball->Position.y += penetration;
                    }
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
        Ball->Stuck = Ball->Sticky;
        SoundEngine->play2D("../resource/bleep.wav", false);
    }

    for (auto itr = this->PowerUps.begin(); itr != this->PowerUps.end(); itr++) {
        PowerUp &item = *itr;
        if (!item.Destroyed) {
            if (item.Position.y > this->height) {
                item.Destroyed = GL_TRUE;
                if (!item.Activated)
                    this->PowerUps.erase(itr);
            }
            if (CheckCollision(item, *Player)) {
                item.Activated = GL_TRUE;
                item.Destroyed = GL_TRUE;
                ActivatePowerUp(item);
                SoundEngine->play2D("../resource/powerup.wav", false);
            }
        }
    }
}

bool CheckCollision(GameObject &one, GameObject &two) {
    bool x_collision = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
    bool y_collision = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
    return x_collision && y_collision;
}

Collision CheckCollision(BallObject &one, GameObject &two) {
    glm::vec2 half_aabb(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 ball_center(one.Position + one.Radius);
    glm::vec2 aabb_center(two.Position + half_aabb);
    if (ball_center.x >= aabb_center.x - half_aabb.x && ball_center.x <= aabb_center.x + half_aabb.x
            && ball_center.y >= aabb_center.y - half_aabb.y && ball_center.y <= aabb_center.y + half_aabb.y) {
        GLfloat x_distance, y_distance;
        if (one.Velocity.x >= 0)
            x_distance = ball_center.x - aabb_center.x + half_aabb.x;
        else
            x_distance = -(aabb_center.x + half_aabb.x - ball_center.x);
        if (one.Velocity.y >= 0)
            y_distance = ball_center.y - aabb_center.y + half_aabb.y;
        else
            y_distance = -(aabb_center.y + half_aabb.y - ball_center.y);
        // 避免除0
        GLfloat backtracking_time;
        if (one.Velocity.x == 0)
            backtracking_time = y_distance / one.Velocity.y;
        else if (one.Velocity.y == 0)
            backtracking_time = x_distance / one.Velocity.x;
        else
            backtracking_time = max(x_distance / one.Velocity.x, y_distance / one.Velocity.y);
        one.Position -= one.Velocity * (backtracking_time + 0.0001f);
        return CheckCollision(one, two);
    }
    glm::vec2 difference = ball_center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -half_aabb, half_aabb);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - ball_center;
    return glm::length(difference) <= one.Radius ? std::make_tuple(GL_TRUE, VectorDirection(difference), difference)
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

