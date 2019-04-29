#include "ListMenu.h"

CListMenu::CListMenu()
    : selectedEntry{0}, entryClicked{false}, selectedColor{1.f, 0.f, 0.f},
      defaultColor{0.f, 1.f, 0.f}, position{0.f, 0.f, 0.f},
      directionModifier{0.f, -1.f, 0.f} {}

CListMenu::~CListMenu() {}

void CListMenu::draw() {
    int i{0};
    for(const auto &entry : menuEntries) {
        entry.model->setRotation(entry.rotation);
        entry.model->setPosition(position+(float)i*directionModifier);
        if (i==selectedEntry)
            g3d::engine->getDefaultShader()->setUniform("colorModulation", selectedColor);
        else
            g3d::engine->getDefaultShader()->setUniform("colorModulation", defaultColor);

        entry.model->draw();
        ++i;
    }
    g3d::engine->getDefaultShader()->setUniform("colorModulation", glm::vec3(1.f,1.f,1.f));
}

void CListMenu::control() {
    if (entryClicked)
        return;

    if (g3d::keyManager.isClicked(SDLK_UP))
        selectedEntry--;
    else if (g3d::keyManager.isClicked(SDLK_DOWN))
        selectedEntry++;

    if (g3d::keyManager.isClicked(SDLK_RETURN))
        entryClicked = true;

    selectedEntry = std::clamp(selectedEntry, 0, (int)(menuEntries.size() - 1));
}

void CListMenu::addEntry(g3d::Model *model, const glm::vec3 &rotation) {
    menuEntries.push_back({model, rotation});
}

void CListMenu::setSelectionColor(const glm::vec3 &color) {
    selectedColor = color;
}

void CListMenu::setDefaultColor(const glm::vec3 &color) {
    defaultColor = color;
}

void CListMenu::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void CListMenu::setDirectionModifier(const glm::vec3 &modifier) {
    directionModifier = modifier;
}

bool CListMenu::IsEntryClicked() {
    if (!entryClicked)
        return false;
    entryClicked = false;
    return true;
}

int CListMenu::getCurrentEntryID() { return selectedEntry; }
