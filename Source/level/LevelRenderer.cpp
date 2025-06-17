#include "LevelRenderer.h"
#include <algorithm>

LevelRenderer::LevelRenderer(Level& level) {
    chunkAmountX = level.width / CHUNK_SIZE;
    chunkAmountY = level.depth / CHUNK_SIZE;
    chunkAmountZ = level.height / CHUNK_SIZE;

    chunks.resize(chunkAmountX * chunkAmountY * chunkAmountZ);

    for (int x = 0; x < chunkAmountX; ++x) {
        for (int y = 0; y < chunkAmountY; ++y) {
            for (int z = 0; z < chunkAmountZ; ++z) {
                int minChunkX = x * CHUNK_SIZE;
                int minChunkY = y * CHUNK_SIZE;
                int minChunkZ = z * CHUNK_SIZE;

                int maxChunkX = std::min(level.width,  (x + 1) * CHUNK_SIZE);
                int maxChunkY = std::min(level.depth,  (y + 1) * CHUNK_SIZE);
                int maxChunkZ = std::min(level.height, (z + 1) * CHUNK_SIZE);

                chunks[(x + y * chunkAmountX) * chunkAmountZ + z] =
                    std::make_unique<Chunk>(level, minChunkX, minChunkY, minChunkZ, maxChunkX, maxChunkY, maxChunkZ);
            }
        }
    }
}

void LevelRenderer::render(int layer) {
    Frustum& frustum = Frustum::getFrustum();
    Chunk::rebuiltThisFrame = 0;

    for (const auto& chunk : chunks) {
        if (frustum.cubeInFrustum(chunk->boundingBox)) {
            chunk->render(layer);
        }
    }
}

void LevelRenderer::setDirty(int minX, int minY, int minZ, int maxX, int maxY, int maxZ) {
    minX /= CHUNK_SIZE;
    minY /= CHUNK_SIZE;
    minZ /= CHUNK_SIZE;
    maxX /= CHUNK_SIZE;
    maxY /= CHUNK_SIZE;
    maxZ /= CHUNK_SIZE;

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);
    minZ = std::max(minZ, 0);
    maxX = std::min(maxX, chunkAmountX - 1);
    maxY = std::min(maxY, chunkAmountY - 1);
    maxZ = std::min(maxZ, chunkAmountZ - 1);

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            for (int z = minZ; z <= maxZ; ++z) {
                auto& chunk = chunks[(x + y * chunkAmountX) * chunkAmountZ + z];
                if (chunk) chunk->setDirty();
            }
        }
    }
}