cmake_minimum_required(VERSION 3.8)
project(BreakOut)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(uber UBER_DIR)
aux_source_directory(util UTIL_DIR)
file(GLOB SHADER_DIR shader *.glsl)
file(GLOB RESOURCE_DIR resource *.*)
file(GLOB CONFIG_DIR levels *.lvl)
set(SOURCE_FILES main.cpp glad.c uber/PowerUp.h)
link_libraries(freetype.lib)
add_executable(BreakOut ${SOURCE_FILES} ${UBER_DIR} ${UTIL_DIR} ${SHADER_DIR} ${RESOURCE_DIR} ${CONFIG_DIR})
target_link_libraries(BreakOut glfw3dll opengl32 irrKlang)