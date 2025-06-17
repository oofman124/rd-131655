#include "Tessellator.h"

Tessellator::Tessellator()
    : vertexBuffer(MAX_VERTICES * 3),
      textureCoordinateBuffer(MAX_VERTICES * 2),
      colorBuffer(MAX_VERTICES * 3),
      vertices(0),
      hasTexture(false),
      textureU(0.0f), textureV(0.0f),
      hasColor(false),
      red(1.0f), green(1.0f), blue(1.0f)
{}

void Tessellator::init() {
    clear();
}

void Tessellator::vertex(float x, float y, float z) {
    vertexBuffer[vertices * 3 + 0] = x;
    vertexBuffer[vertices * 3 + 1] = y;
    vertexBuffer[vertices * 3 + 2] = z;

    if (hasTexture) {
        textureCoordinateBuffer[vertices * 2 + 0] = textureU;
        textureCoordinateBuffer[vertices * 2 + 1] = textureV;
    }
    if (hasColor) {
        colorBuffer[vertices * 3 + 0] = red;
        colorBuffer[vertices * 3 + 1] = green;
        colorBuffer[vertices * 3 + 2] = blue;
    }

    vertices++;
    if (vertices == MAX_VERTICES) {
        flush();
    }
}

void Tessellator::texture(float u, float v) {
    hasTexture = true;
    textureU = u;
    textureV = v;
}

void Tessellator::color(float r, float g, float b) {
    hasColor = true;
    red = r;
    green = g;
    blue = b;
}

void Tessellator::flush() {
    if (vertices == 0) return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexBuffer.data());

    if (hasTexture) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, textureCoordinateBuffer.data());
    }
    if (hasColor) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colorBuffer.data());
    }

    glDrawArrays(GL_QUADS, 0, vertices);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (hasTexture) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (hasColor) glDisableClientState(GL_COLOR_ARRAY);

    clear();
}

void Tessellator::clear() {
    vertices = 0;
    hasTexture = false;
    hasColor = false;
}