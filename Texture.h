#pragma once
#include <GL/glew.h>
#include <string>

namespace g3d {

class Texture;

class TextureBuilder {
    Texture *texture;
    uint8_t *raw;
    bool loadedFromMemory;
    bool generateMipMaps;
    GLint wrap, filter;
    GLuint textureID;
    int w, h;

public:
    TextureBuilder();
    ~TextureBuilder();
    TextureBuilder &setWrap(GLint value);
    TextureBuilder &setFilter(GLint value);
    TextureBuilder &generateMipmaps(bool value);
    TextureBuilder &fromFile(std::string fileName);
    TextureBuilder &fromRawRGBA(uint8_t *raw, int w, int h);
    Texture *get();
    Texture *build();
};

class Texture {
    GLuint textureID;

public:
    Texture(GLuint tex);
    Texture();
    ~Texture();
    void set(GLuint tex);
    void bind(GLuint texUnit = 0);
    void unbind(GLuint texUnit = 0);

    static TextureBuilder create();
};

};
