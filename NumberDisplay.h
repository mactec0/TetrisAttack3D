#pragma once
#include "Engine.h"
#include <string>

class CNumberDisplay {
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;
    g3d::Model *models[10];
    bool alignFromLeft;
    std::string text;
    const float letterWidth;
    int characterToNumber(char c);
    void drawNumber(const glm::vec3 &pos, int ID);

public:
    CNumberDisplay(bool align = true);
    ~CNumberDisplay();

    void setText(const std::string &str);
    void setText(const int number);
    std::string getText();

    void setAlign(bool align);

    void setColor(const glm::vec3 &color);
    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::vec3 &rotation);
    void setScale(const float scale);

    void draw();
};
