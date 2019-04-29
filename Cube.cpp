#include "Cube.h"

CCube::CCube() {
    model = g3d::resourceManager.get("box_model");
    for (int i = 0; i < BOX_COLOR_MAX; ++i) {
        textures[i] = g3d::resourceManager.get("box_texture[" +
                                               std::to_string(i + 1) + "]");
    }
    color = BOX_COLOR_1;
}

CCube::~CCube() {}

void CCube::setPosition(const glm::vec3 &pos) { model->setPosition(pos); }

void CCube::setColor(BOX_COLOR color) {
    this->color = color;
    model->setTexture(textures[color]);
}
BOX_COLOR CCube::getColor() { return color; }

void CCube::draw() { model->draw(); }
