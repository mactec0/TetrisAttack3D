#pragma once
#include "Engine.h"
#include <algorithm>
#include <vector>

class CListMenu {
    struct SMenuEntry {
        g3d::Model *model;
        glm::vec3 rotation;
    };
    std::vector<SMenuEntry> menuEntries;
    int selectedEntry;
    bool entryClicked;
    glm::vec3 selectedColor, defaultColor;
    glm::vec3 position;
    glm::vec3 directionModifier;

public:
    CListMenu();
    ~CListMenu();

    void draw();
    void control();

    void addEntry(g3d::Model *model,
                  const glm::vec3 &rotation = {0.f, 0.f, 0.f});

    void setSelectionColor(const glm::vec3 &color);
    void setDefaultColor(const glm::vec3 &color);

    void setPosition(const glm::vec3 &position);

    void setDirectionModifier(const glm::vec3 &modifier);

    bool IsEntryClicked();

    int getCurrentEntryID();
};
