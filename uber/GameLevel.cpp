//
// Created by dell on 2017/11/13.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "GameLevel.h"
#include "../util/ResourceManager.h"

using namespace std;

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight) {
    this->Bricks.clear();
    GLuint tileCode;
    GameLevel level;
    string line;
    ifstream fstream(file);
    vector<vector<GLuint >> tileData;

    if(fstream) {
        while (getline(fstream, line)) {
            // 将string变为stream，便可以用>>轻松按空格分隔数字
            istringstream sstream(line);
            vector<GLuint> row;
            while (sstream >> tileCode)
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (!tileData.empty())
            this -> init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer) {
    for (GameObject &tile: this->Bricks) {
        if (!tile.Destroyed)
            tile.draw(renderer);
    }
}

GLboolean GameLevel::IsComplete() {}

void GameLevel::init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
    GLuint height = tileData.size();
    GLuint width = tileData[0].size();
    GLfloat brick_width = levelWidth / (GLfloat) width;
    GLfloat brick_height = levelHeight / (GLfloat) height;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            glm::vec2 pos(brick_width * j, brick_height * i);
            glm::vec2 size(brick_width, brick_height);

            if (tileData[i][j] == 1) {
                GameObject obj(pos, size, ResourceManager::getTexture("block_solid"));
                obj.IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if (tileData[i][j] > 1) {
                glm::vec3 color(1.0f);
                if (tileData[i][j] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[i][j] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[i][j] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[i][j] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                // 用emplace_back替代push_back，前者可以省去一次拷贝过程（？）
                this->Bricks.emplace_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));
            }
        }
    }
}