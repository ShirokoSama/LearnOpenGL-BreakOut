cmake_minimum_required(VERSION 3.8)
project(BreakOut)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp glad.c uber/Game.cpp uber/Game.h util/Shader.cpp util/Shader.h
        util/Texture2D.cpp util/Texture2D.h util/ResourceManager.cpp util/ResourceManager.h util/stb_image.cpp
        util/SpriteRenderer.cpp util/SpriteRenderer.h shader/sprite_fs.glsl shader/sprite_vs.glsl
        resource/awesomeface.png)
add_executable(BreakOut ${SOURCE_FILES})
target_link_libraries(BreakOut glfw3dll opengl32)