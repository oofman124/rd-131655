#pragma once

#include "Tessellator.h"
#include "Level.h"

class Tile {
public:
    static Tile grass;
    static Tile rock;

    int textureId;

    Tile(int textureId) : textureId(textureId) {}

    void render(Tessellator& tessellator, Level& level, int layer, int x, int y, int z) {
        float minU = textureId / 16.0f;
        float maxU = minU + 16.0f / 256.0f;
        float minV = 0.0f;
        float maxV = minV + 16.0f / 256.0f;

        float shadeX = 0.6f;
        float shadeY = 1.0f;
        float shadeZ = 0.8f;

        float minX = x + 0.0f;
        float maxX = x + 1.0f;
        float minY = y + 0.0f;
        float maxY = y + 1.0f;
        float minZ = z + 0.0f;
        float maxZ = z + 1.0f;

        // Bottom face
        if (!level.isSolidTile(x, y - 1, z)) {
            float brightness = level.getBrightness(x, y - 1, z) * shadeY;
            if ((layer == 1) ^ (brightness == shadeY)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(minU, maxV); tessellator.vertex(minX, minY, maxZ);
                tessellator.texture(minU, minV); tessellator.vertex(minX, minY, minZ);
                tessellator.texture(maxU, minV); tessellator.vertex(maxX, minY, minZ);
                tessellator.texture(maxU, maxV); tessellator.vertex(maxX, minY, maxZ);
            }
        }
        // Top face
        if (!level.isSolidTile(x, y + 1, z)) {
            float brightness = level.getBrightness(x, y + 1, z) * shadeY;
            if ((layer == 1) ^ (brightness == shadeY)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(maxU, maxV); tessellator.vertex(maxX, maxY, maxZ);
                tessellator.texture(maxU, minV); tessellator.vertex(maxX, maxY, minZ);
                tessellator.texture(minU, minV); tessellator.vertex(minX, maxY, minZ);
                tessellator.texture(minU, maxV); tessellator.vertex(minX, maxY, maxZ);
            }
        }
        // Side faces Z
        if (!level.isSolidTile(x, y, z - 1)) {
            float brightness = level.getBrightness(x, y, z - 1) * shadeZ;
            if ((layer == 1) ^ (brightness == shadeZ)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(maxU, minV); tessellator.vertex(minX, maxY, minZ);
                tessellator.texture(minU, minV); tessellator.vertex(maxX, maxY, minZ);
                tessellator.texture(minU, maxV); tessellator.vertex(maxX, minY, minZ);
                tessellator.texture(maxU, maxV); tessellator.vertex(minX, minY, minZ);
            }
        }
        if (!level.isSolidTile(x, y, z + 1)) {
            float brightness = level.getBrightness(x, y, z + 1) * shadeZ;
            if ((layer == 1) ^ (brightness == shadeZ)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(minU, minV); tessellator.vertex(minX, maxY, maxZ);
                tessellator.texture(minU, maxV); tessellator.vertex(minX, minY, maxZ);
                tessellator.texture(maxU, maxV); tessellator.vertex(maxX, minY, maxZ);
                tessellator.texture(maxU, minV); tessellator.vertex(maxX, maxY, maxZ);
            }
        }
        // Side faces X
        if (!level.isSolidTile(x - 1, y, z)) {
            float brightness = level.getBrightness(x - 1, y, z) * shadeX;
            if ((layer == 1) ^ (brightness == shadeX)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(maxU, minV); tessellator.vertex(minX, maxY, maxZ);
                tessellator.texture(minU, minV); tessellator.vertex(minX, maxY, minZ);
                tessellator.texture(minU, maxV); tessellator.vertex(minX, minY, minZ);
                tessellator.texture(maxU, maxV); tessellator.vertex(minX, minY, maxZ);
            }
        }
        if (!level.isSolidTile(x + 1, y, z)) {
            float brightness = level.getBrightness(x + 1, y, z) * shadeX;
            if ((layer == 1) ^ (brightness == shadeX)) {
                tessellator.color(brightness, brightness, brightness);
                tessellator.texture(minU, maxV); tessellator.vertex(maxX, minY, maxZ);
                tessellator.texture(maxU, maxV); tessellator.vertex(maxX, minY, minZ);
                tessellator.texture(maxU, minV); tessellator.vertex(maxX, maxY, minZ);
                tessellator.texture(minU, minV); tessellator.vertex(maxX, maxY, maxZ);
            }
        }
    }
};

// Static member definitions
inline Tile Tile::grass = Tile(0);
inline Tile Tile::rock = Tile(1);