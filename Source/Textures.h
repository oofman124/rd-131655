#pragma once

#include <string>

class Textures {
public:
    // Loads a texture from file and returns the OpenGL texture ID.
    // mode: GL_NEAREST or GL_LINEAR
    static int loadTexture(const std::string& filename, int mode);

    // Binds the texture if not already bound
    static void bind(int id);

private:
    static int lastId;
};