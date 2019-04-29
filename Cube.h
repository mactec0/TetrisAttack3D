#pragma once
#include "Engine.h"

 enum BOX_COLOR{
    BOX_COLOR_1,
    BOX_COLOR_2,
    BOX_COLOR_3,
    BOX_COLOR_4,
    BOX_COLOR_5,
    BOX_COLOR_MAX,
    BOX_FALLING,
    BOX_FALLING_COLOR_1=BOX_FALLING,
    BOX_FALLING_COLOR_2,
    BOX_FALLING_COLOR_3,
    BOX_FALLING_COLOR_4,
    BOX_FALLING_COLOR_5

};

class CCube {
    g3d::Model *model;
    g3d::Texture *textures[BOX_COLOR_MAX];
    BOX_COLOR color;

 public:
    CCube();
    ~CCube();

    void setColor(BOX_COLOR color);
    BOX_COLOR getColor();

    void setPosition(const glm::vec3 &pos);

    void draw();
};
