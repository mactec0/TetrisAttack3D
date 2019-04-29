#pragma once
#include "Engine.h"
#include "KeyManager.h"
#include <algorithm>

class CBlockSwapper {
    g3d::Model *model;
    glm::vec3 boardOffset;
    glm::ivec2 position;

 public:
    CBlockSwapper();
    ~CBlockSwapper();

    void Control();
    void draw(float scroll);

    void setBoardOffset(const glm::vec3 &offset);

    void clamp();

    void setPosition(int x, int y);

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void reset();

    glm::ivec2 getPositionL();
    glm::ivec2 getPositionR();
};
