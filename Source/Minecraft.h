#pragma once
#ifndef MINECRAFT_H
#define MINECRAFT_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "Lib/Timer.h"
#include "level/Level.h"
#include "level/LevelRenderer.h"
#include "Player.h"

class Minecraft {
    Timer timer;
    Level* level;
    LevelRenderer* levelRenderer;
    Player* player;
    float fogColor[4];
    GLFWwindow* window;

public:
    Minecraft();
    ~Minecraft();

    void init();
    void destroy();
    void run();
    void tick();
    void moveCameraToPlayer(float partialTicks);
    void render(float partialTicks);
};

#endif // MINECRAFT_H