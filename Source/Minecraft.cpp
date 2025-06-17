#include "Minecraft.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"
#include <glm/gtc/type_ptr.hpp>



Minecraft::Minecraft()
    : timer(60), level(nullptr), levelRenderer(nullptr), player(nullptr), window(nullptr)
{
    fogColor[0] = 14.0f / 255.0f;
    fogColor[1] = 11.0f / 255.0f;
    fogColor[2] = 10.0f / 255.0f;
    fogColor[3] = 1.0f;
}

Minecraft::~Minecraft() {
    destroy();
}

void Minecraft::init() {
    int width = 1024, height = 768;

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    window = glfwCreateWindow(width, height, "Minecraft", nullptr, nullptr);
    g_window = window; // Set global window pointer for input handling
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.8f, 1.0f, 0.0f);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = width / (float)height;
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), aspect, 0.05f, 1000.0f);
    glLoadMatrixf(glm::value_ptr(proj));
    glMatrixMode(GL_MODELVIEW);

    level = new Level(256, 256, 64);
    levelRenderer = new LevelRenderer(*level);
    player = new Player(*level);
}

void Minecraft::destroy() {
    if (level) level->save();
    delete player;
    delete levelRenderer;
    delete level;
    player = nullptr;
    levelRenderer = nullptr;
    level = nullptr;
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Minecraft::run() {
    std::cout << "Starting Minecraft..." << std::endl;
    try {
        init();
    } catch (const std::exception& e) {
        std::cerr << "Failed to start Minecraft: " << e.what() << std::endl;
        return;
    }
    std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
    int frames = 0;
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        timer.advanceTime();
        for (int i = 0; i < timer.ticks; ++i) {
            tick();
        }
        render(timer.partialTicks);
        frames++;

        double now = glfwGetTime();
        if (now - lastTime >= 1.0) {
            std::cout << frames << " fps, " << Chunk::updates << std::endl;
            Chunk::updates = 0;
            frames = 0;
            lastTime += 1.0;
        }
        glfwPollEvents();
    }
    std::cout << "Exiting..." << std::endl;
    destroy();
}

void Minecraft::tick() {
    player->tick();
}

void Minecraft::moveCameraToPlayer(float partialTicks) {
    glTranslatef(0.0f, 0.0f, -0.3f);
    glRotatef(player->xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(player->yRotation, 0.0f, 1.0f, 0.0f);

    double x = player->prevX + (player->x - player->prevX) * partialTicks;
    double y = player->prevY + (player->y - player->prevY) * partialTicks;
    double z = player->prevZ + (player->z - player->prevZ) * partialTicks;
    glTranslated(-x, -y, -z);
}

void Minecraft::render(float partialTicks) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    static double lastX = xpos, lastY = ypos;
    float motionX = float(xpos - lastX);
    float motionY = -float(ypos - lastY);
    lastX = xpos; lastY = ypos;

    player->turn(motionX, motionY);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    moveCameraToPlayer(partialTicks);

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, -10);
    glFogf(GL_FOG_END, 20);
    glFogfv(GL_FOG_COLOR, fogColor);
    glDisable(GL_FOG);

    levelRenderer->render(0);

    glEnable(GL_FOG);
    levelRenderer->render(1);

    glDisable(GL_TEXTURE_2D);

    glfwSwapBuffers(window);
}