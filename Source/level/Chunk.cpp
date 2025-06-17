#include "Chunk.h"
#include "../Textures.h"

int Chunk::rebuiltThisFrame = 0;
int Chunk::updates = 0;
int Chunk::TEXTURE = 0;
Tessellator Chunk::TESSELLATOR;

Chunk::Chunk(Level& level, int minX, int minY, int minZ, int maxX, int maxY, int maxZ)
    : level(level),
      boundingBox(minX, minY, minZ, maxX, maxY, maxZ),
      minX(minX), minY(minY), minZ(minZ),
      maxX(maxX), maxY(maxY), maxZ(maxZ),
      dirty(true)
{
    // Only load texture once
    if (TEXTURE == 0)
        TEXTURE = Textures::loadTexture("resources/terrain.png", GL_NEAREST);

    lists = glGenLists(2);
}

Chunk::~Chunk() {
    glDeleteLists(lists, 2);
}

void Chunk::rebuild(int layer) {
    if (rebuiltThisFrame == 2) return;
    updates++;
    rebuiltThisFrame++;
    dirty = false;

    glNewList(lists + layer, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TEXTURE);
    TESSELLATOR.init();

    for (int x = minX; x < maxX; ++x) {
        for (int y = minY; y < maxY; ++y) {
            for (int z = minZ; z < maxZ; ++z) {
                if (level.isTile(x, y, z)) {
                    if (y > level.depth - 7 && level.getBrightness(x, y, z) == 1.0f) {
                        Tile::grass.render(TESSELLATOR, level, layer, x, y, z);
                    } else {
                        Tile::rock.render(TESSELLATOR, level, layer, x, y, z);
                    }
                }
            }
        }
    }

    TESSELLATOR.flush();
    glDisable(GL_TEXTURE_2D);
    glEndList();
}

void Chunk::render(int layer) {
    if (dirty) {
        rebuild(0);
        rebuild(1);
    }
    glCallList(lists + layer);
}

void Chunk::setDirty() {
    dirty = true;
}