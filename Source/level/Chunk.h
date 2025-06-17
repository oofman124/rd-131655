#pragma once
#ifndef CHUNK_H
#define CHUNK_H
#include "Level.h"
#include "../phys/AABB.h"
#include "Tessellator.h"
#include "Tile.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Chunk {
public:
    static int rebuiltThisFrame;
    static int updates;

    Level& level;
    AABB boundingBox;
    const int minX, minY, minZ;
    const int maxX, maxY, maxZ;

    Chunk(Level& level, int minX, int minY, int minZ, int maxX, int maxY, int maxZ);
    ~Chunk();

    void rebuild(int layer);
    void render(int layer);
    void setDirty();

private:
    int lists;
    bool dirty;
    static int TEXTURE;
    static Tessellator TESSELLATOR;
};

#endif // CHUNK_H