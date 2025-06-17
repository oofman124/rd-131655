#pragma once

#include "level/Level.h"
#include "phys/AABB.h"
#include <memory>

class Player {
public:
    Level& level;

    double x, y, z;
    double prevX, prevY, prevZ;
    double motionX, motionY, motionZ;
    float xRotation, yRotation;

    bool onGround;
    AABB boundingBox;

    Player(Level& level);

    void turn(float x, float y);
    void tick();

    // For movement and collision
    void move(double x, double y, double z);

private:
    void setPosition(float x, float y, float z);
    void resetPosition();
    void moveRelative(float x, float z, float speed);
};