#include "Player.h"
#include <algorithm>
#include <cmath>
#include <random>
#include "input.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static std::random_device rd;
static std::mt19937 rng(rd());


Player::Player(Level& level)
    : level(level), x(0), y(0), z(0), prevX(0), prevY(0), prevZ(0),
      motionX(0), motionY(0), motionZ(0), xRotation(0), yRotation(0),
      onGround(false), boundingBox(0,0,0,0,0,0)
{
    resetPosition();
}

void Player::setPosition(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    float width = 0.3f;
    float height = 0.9f;
    boundingBox = AABB(x - width, y - height, z - width, x + width, y + height, z + width);
}

void Player::resetPosition() {
    std::uniform_real_distribution<float> distX(0.0f, static_cast<float>(level.width));
    std::uniform_real_distribution<float> distY(static_cast<float>(level.depth - 3), static_cast<float>(level.depth + 3));
    std::uniform_real_distribution<float> distZ(0.0f, static_cast<float>(level.height));
    float x = distX(rng);
    float y = distY(rng);
    float z = distZ(rng);
    setPosition(x, y, z);
}

void Player::turn(float x, float y) {
    yRotation += x * 0.15f;
    xRotation -= y * 0.15f;
    xRotation = std::max(-90.0f, std::min(90.0f, xRotation));
}

void Player::tick() {
    prevX = x;
    prevY = y;
    prevZ = z;

    float forward = 0.0f;
    float vertical = 0.0f;

    // Replace with your own key input system
    if (isKeyDown(GLFW_KEY_R)) { // R
        resetPosition();
    }
    if (isKeyDown(GLFW_KEY_UP) || isKeyDown(GLFW_KEY_W)) { // Up, W
        forward--;
    }
    if (isKeyDown(GLFW_KEY_DOWN) || isKeyDown(GLFW_KEY_S)) { // Down, S
        forward++;
    }
    if (isKeyDown(GLFW_KEY_LEFT) || isKeyDown(GLFW_KEY_A)) { // Left, A
        vertical--;
    }
    if (isKeyDown(GLFW_KEY_RIGHT) || isKeyDown(GLFW_KEY_D)) { // Right, D
        vertical++;
    }
    if (isKeyDown(GLFW_KEY_SPACE) || isKeyDown(GLFW_KEY_LEFT_SUPER)) { // Space, Windows Key
        if (onGround) {
            motionY = 0.12f;
        }
    }
    

    moveRelative(vertical, forward, onGround ? 0.02f : 0.005f);

    motionY -= 0.005;

    move(motionX, motionY, motionZ);

    motionX *= 0.91f;
    motionY *= 0.98f;
    motionZ *= 0.91f;

    if (onGround) {
        motionX *= 0.8f;
        motionZ *= 0.8f;
    }
}

void Player::move(double x, double y, double z) {
    double prevX = x, prevY = y, prevZ = z;
    auto aabbs = level.getCubes(boundingBox.expand(x, y, z));

    for (const auto& abb : aabbs) {
        y = abb.clipYCollide(boundingBox, y);
    }
    boundingBox.move(0.0, y, 0.0);

    for (const auto& abb : aabbs) {
        x = abb.clipXCollide(boundingBox, x);
    }
    boundingBox.move(x, 0.0, 0.0);

    for (const auto& abb : aabbs) {
        z = abb.clipZCollide(boundingBox, z);
    }
    boundingBox.move(0.0, 0.0, z);

    onGround = prevY != y && prevY < 0.0;

    if (prevX != x) motionX = 0.0;
    if (prevY != y) motionY = 0.0;
    if (prevZ != z) motionZ = 0.0;

    this->x = (boundingBox.minX + boundingBox.maxX) / 2.0;
    this->y = boundingBox.minY + 1.62;
    this->z = (boundingBox.minZ + boundingBox.maxZ) / 2.0;
}

void Player::moveRelative(float x, float z, float speed) {
    float dist = x * x + z * z;
    if (dist < 0.01f) return;
    dist = speed / std::sqrt(dist);
    x *= dist;
    z *= dist;

    double sin = std::sin(yRotation * M_PI / 180.0);
    double cos = std::cos(yRotation * M_PI / 180.0);

    motionX += x * cos - z * sin;
    motionZ += z * cos + x * sin;
}