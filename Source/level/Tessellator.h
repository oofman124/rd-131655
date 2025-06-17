#pragma once

#include <vector>
#include <glad/glad.h>

class Tessellator {
public:
    static constexpr int MAX_VERTICES = 100000;

    Tessellator();

    void init();
    void vertex(float x, float y, float z);
    void texture(float u, float v);
    void color(float r, float g, float b);
    void flush();

private:
    std::vector<float> vertexBuffer;
    std::vector<float> textureCoordinateBuffer;
    std::vector<float> colorBuffer;

    int vertices;

    // Texture
    bool hasTexture;
    float textureU, textureV;

    // Color
    bool hasColor;
    float red, green, blue;

    void clear();
};