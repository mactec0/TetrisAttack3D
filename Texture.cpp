#include "Texture.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace g3d{

TextureBuilder::TextureBuilder()
    : texture{new Texture()}, loadedFromMemory{false},
      generateMipMaps{false}, wrap{GL_REPEAT}, filter{GL_LINEAR} {}

TextureBuilder::~TextureBuilder() {}

TextureBuilder &TextureBuilder::setWrap(GLint value) {
    wrap = value;
    return *this;
}

TextureBuilder &TextureBuilder::setFilter(GLint value) {
    filter = value;
    return *this;
}

TextureBuilder &TextureBuilder::generateMipmaps(bool value) {
    generateMipMaps = value;
    return *this;
}

TextureBuilder &TextureBuilder::fromFile(std::string fileName) {
    stbi_set_flip_vertically_on_load(GL_TRUE);
    int components;
    raw = stbi_load(fileName.c_str(), &w, &h, &components, STBI_rgb_alpha);
    if (!raw) {
        std::cerr << "[ERROR] Cannot load {" << fileName << "}\n";
        exit(0);
    }
    return *this;
}

TextureBuilder &TextureBuilder::fromRawRGBA(uint8_t *raw, int w, int h) {
    this->raw = raw;
    this->w = w;
    this->h = h;
    loadedFromMemory = true;
    return *this;
}

Texture *TextureBuilder::get() { return texture; }

Texture *TextureBuilder::build() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, raw);

    if (generateMipMaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
    }

    if (!loadedFromMemory)
        stbi_image_free(raw);

    glBindTexture(GL_TEXTURE_2D, 0);

    texture = new Texture(textureID);

    return texture;
}

Texture::Texture(GLuint tex) { this->set(tex); }

Texture::Texture() : textureID(0) {}

Texture::~Texture() { glDeleteTextures(1, &textureID); }


TextureBuilder Texture::create(){
	return TextureBuilder();
}

void Texture::set(GLuint tex){
	textureID=tex;
}

void Texture::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
}



};
