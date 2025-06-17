#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include "../phys/AABB.h"

class Level {
public:
    const int width, height, depth;

    Level(int width, int height, int depth);
    void load();
    void save();
    float getBrightness(int x, int y, int z) const;
    bool isTile(int x, int y, int z) const;
    bool isSolidTile(int x, int y, int z) const;
    bool isLightBlocker(int x, int y, int z) const;
    std::vector<AABB> getCubes(const AABB& boundingBox) const;

private:
    std::vector<uint8_t> blocks;
    std::vector<int> lightDepths;
    void calcLightDepths(int minX, int minZ, int maxX, int maxZ);
};

#endif // LEVEL_H