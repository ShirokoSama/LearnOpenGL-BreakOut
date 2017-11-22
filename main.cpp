//
// Created by Srf on 2017/10/5.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "uber/Game.h"
#include "util/ResourceManager.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;

const GLuint SCREEN_HEIGHT = 600;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetKeyCallback(window, key_callback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Breakout.init();

    GLfloat deltaTime;
    GLfloat lastFrame = 0.0f;
    Breakout.state = GAME_ACTIVE;

    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        Breakout.processInput(deltaTime);
        Breakout.update(deltaTime);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.render();
        glfwSwapBuffers(window);
    }
    ResourceManager::clear();
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            Breakout.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            Breakout.keys[key] = GL_FALSE;
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
        {
            Breakout.keys[key] = GL_FALSE;
            Breakout.KeysProcessed[key] = GL_FALSE;
        }
    }
}

