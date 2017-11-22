cmake_minimum_required(VERSION 3.8)
project(BreakOut)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp glad.c
        uber/Game.cpp uber/Game.h util/Shader.cpp util/Shader.h
        util/Texture2D.cpp util/Texture2D.h util/ResourceManager.cpp util/ResourceManager.h util/stb_image.cpp
        util/SpriteRenderer.cpp util/SpriteRenderer.h shader/sprite_fs.glsl shader/sprite_vs.glsl
        resource/awesomeface.png uber/GameObject.cpp uber/GameObject.h uber/GameLevel.cpp uber/GameLevel.h
        levels/one.lvl levels/two.lvl levels/three.lvl levels/four.lvl
        resource/background.jpg resource/block.png resource/block_solid.png resource/paddle.png
        uber/BallObject.cpp uber/BallObject.h uber/ParticleGenerator.cpp uber/ParticleGenerator.h
        resource/particle.png uber/PostProcessor.cpp uber/PostProcessor.h uber/PowerUp.h
        resource/powerup_chaos.png resource/powerup_confuse.png resource/powerup_increase.png
        resource/powerup_passthrough.png resource/powerup_speed.png resource/powerup_sticky.png
        resource/bleep.mp3 resource/bleep.wav resource/breakout.mp3 resource/powerup.wav resource/solid.wav
        util/TextRenderer.cpp util/TextRenderer.h
        shader/text_vs.glsl shader/text_fs.glsl
        )
link_libraries(freetype.lib)
add_executable(BreakOut ${SOURCE_FILES})
target_link_libraries(BreakOut glfw3dll opengl32 irrKlang)