#include "Level.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <random>
#include <zlib.h> // For gzFile, gzopen, gzread, gzwrite, gzclose

Level::Level(int width, int height, int depth)
    : width(width), height(height), depth(depth),
      blocks(width * height * depth, 0),
      lightDepths(width * height, 0)
{
    std::ifstream levelFile("level.dat", std::ios::binary);
    if (levelFile.good()) {
        load();
        return;
    }

    // Fill level with tiles
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < depth; ++y) {
            for (int z = 0; z < height; ++z) {
                int index = (y * this->height + z) * this->width + x;
                blocks[index] = 1;
            }
        }
    }

    // Generate caves
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int i = 0; i < 10000; ++i) {
        int caveSize = static_cast<int>(dist(rng) * 7) + 1;
        int caveX = static_cast<int>(dist(rng) * width);
        int caveY = static_cast<int>(dist(rng) * depth);
        int caveZ = static_cast<int>(dist(rng) * height);

        for (int radius = 0; radius < caveSize; ++radius) {
            for (int sphere = 0; sphere < 1000; ++sphere) {
                int offsetX = static_cast<int>(dist(rng) * radius * 2 - radius);
                int offsetY = static_cast<int>(dist(rng) * radius * 2 - radius);
                int offsetZ = static_cast<int>(dist(rng) * radius * 2 - radius);

                double distance = offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ;
                if (distance > radius * radius)
                    continue;

                int tileX = caveX + offsetX;
                int tileY = caveY + offsetY;
                int tileZ = caveZ + offsetZ;

                int index = (tileY * this->height + tileZ) * this->width + tileX;
                if (tileX > 0 && tileY > 0 && tileZ > 0
                    && tileX < this->width - 1 && tileY < this->depth && tileZ < this->height - 1
                    && index >= 0 && index < static_cast<int>(blocks.size())) {
                    blocks[index] = 0;
                }
            }
        }
    }

    calcLightDepths(0, 0, width, height);
}

void Level::load() {
    gzFile file = gzopen("level.dat", "rb");
    if (!file) return;
    gzread(file, blocks.data(), blocks.size());
    gzclose(file);
    calcLightDepths(0, 0, width, height);
}

void Level::save() {
    gzFile file = gzopen("level.dat", "wb");
    if (!file) return;
    gzwrite(file, blocks.data(), blocks.size());
    gzclose(file);
}

void Level::calcLightDepths(int minX, int minZ, int maxX, int maxZ) {
    for (int x = minX; x < minX + maxX; ++x) {
        for (int z = minZ; z < minZ + maxZ; ++z) {
            int prevDepth = lightDepths[x + z * width];
            int depthVal = depth - 1;
            while (depthVal > 0 && !isLightBlocker(x, depthVal, z)) {
                depthVal--;
            }
            lightDepths[x + z * width] = depthVal;
        }
    }
}

bool Level::isTile(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0 || x >= width || y >= depth || z >= height)
        return false;
    int index = (y * height + z) * width + x;
    return blocks[index] != 0;
}

bool Level::isSolidTile(int x, int y, int z) const {
    return isTile(x, y, z);
}

bool Level::isLightBlocker(int x, int y, int z) const {
    return isSolidTile(x, y, z);
}

float Level::getBrightness(int x, int y, int z) const {
    float dark = 0.8f, light = 1.0f;
    if (x < 0 || y < 0 || z < 0 || x >= width || y >= depth || z >= height)
        return light;
    if (y < lightDepths[x + z * width])
        return dark;
    return light;
}

std::vector<AABB> Level::getCubes(const AABB& boundingBox) const {
    std::vector<AABB> cubes;
    int minX = static_cast<int>(std::floor(boundingBox.minX)) - 1;
    int maxX = static_cast<int>(std::ceil(boundingBox.maxX)) + 1;
    int minY = static_cast<int>(std::floor(boundingBox.minY)) - 1;
    int maxY = static_cast<int>(std::ceil(boundingBox.maxY)) + 1;
    int minZ = static_cast<int>(std::floor(boundingBox.minZ)) - 1;
    int maxZ = static_cast<int>(std::ceil(boundingBox.maxZ)) + 1;

    minX = std::max(0, minX);
    minY = std::max(0, minY);
    minZ = std::max(0, minZ);
    maxX = std::min(width, maxX);
    maxY = std::min(depth, maxY);
    maxZ = std::min(height, maxZ);

    for (int x = minX; x < maxX; ++x) {
        for (int y = minY; y < maxY; ++y) {
            for (int z = minZ; z < maxZ; ++z) {
                if (isSolidTile(x, y, z)) {
                    cubes.emplace_back(x, y, z, x + 1, y + 1, z + 1);
                }
            }
        }
    }
    return cubes;
}