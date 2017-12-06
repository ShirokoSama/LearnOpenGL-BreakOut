//
// Created by dell on 2017/11/13.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
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

void GameLevel::init(vector<vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
    row_num = tileData.size();
    column_num = tileData[0].size();
    grid_width = levelWidth / (GLfloat) column_num;
    grid_height = levelHeight / (GLfloat) row_num;
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < column_num; j++) {
            glm::vec2 pos(grid_width * j, grid_height * i);
            glm::vec2 size(grid_width, grid_height);

            if (tileData[i][j] == 0) {
                GameObject obj = GameObject();
                obj.Destroyed = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if (tileData[i][j] == 1) {
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

bool GameLevel::IsComplete() {
    for (GameObject &obj: this->Bricks) {
        if (!obj.IsSolid && !obj.Destroyed)
            return false;
    }
    return true;
}

vector<GameObject*> GameLevel::CollisionBroadPhase(GameObject &obj) {
    auto x_start_index = (int)(obj.Position.x / grid_width);
    auto x_end_index = (int)((obj.Position.x + obj.Size.x) / grid_width);
    auto y_start_index = (int)(obj.Position.y / grid_height);
    auto y_end_index = (int)((obj.Position.y + obj.Size.y) / grid_height);
    vector<GameObject*> rslt;
    for (int i = y_start_index; i <= y_end_index; i++) {
        for (int j = x_start_index; j <= x_end_index; j++) {
            rslt.push_back(&this->Bricks[i * column_num + j]);
        }
    }
    return rslt;
}