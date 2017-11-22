//
// Created by dell on 2017/11/13.
//

#ifndef BREAKOUT_GAMELEVEL_H
#define BREAKOUT_GAMELEVEL_H

#include <vector>
#include "GameObject.h"

using namespace std;

class GameLevel {
public:
    vector<GameObject> Bricks;
    GameLevel() = default;
    void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
    void Draw(SpriteRenderer &renderer);
    bool IsComplete();
private:
    void init(vector<vector<GLuint>> titleData, GLuint levelWidth, GLuint levelHeight);
};


#endif //BREAKOUT_GAMELEVEL_H
