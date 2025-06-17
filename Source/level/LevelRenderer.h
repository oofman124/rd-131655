#pragma once

#include "Level.h"
#include "Chunk.h"
#include "Frustum.h"
#include <vector>
#include <memory>

class LevelRenderer {
public:
    static constexpr int CHUNK_SIZE = 8;

    LevelRenderer(Level& level);

    void render(int layer);
    void setDirty(int minX, int minY, int minZ, int maxX, int maxY, int maxZ);

private:
    std::vector<std::unique_ptr<Chunk>> chunks;
    int chunkAmountX, chunkAmountY, chunkAmountZ;
};