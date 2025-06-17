#include "Textures.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

int Textures::lastId = std::numeric_limits<int>::min();

int Textures::loadTexture(const std::string& filename, int mode) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!data) {
        throw std::runtime_error("Could not load texture: " + filename);
    }

    GLuint id = 0;
    glGenTextures(1, &id);
    bind(id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return id;
}

void Textures::bind(int id) {
    if (id != lastId) {
        glBindTexture(GL_TEXTURE_2D, id);
        lastId = id;
    }
}