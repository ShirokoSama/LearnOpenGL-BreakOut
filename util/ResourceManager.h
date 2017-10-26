//
// Created by Srf on 2017/10/5.
//

#ifndef BREAKOUT_RESOURCEMANAGER_H
#define BREAKOUT_RESOURCEMANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include "Texture2D.h"
#include "Shader.h"

class ResourceManager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;
    static Shader loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader getShader(std::string name);
    static Texture2D loadTexture(const GLchar *file, GLboolean alpha, std::string name);
    static Texture2D getTexture(std::string name);
    static void clear();
private:
    ResourceManager() = default;
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};


#endif //BREAKOUT_RESOURCEMANAGER_H
