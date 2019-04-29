#include "BlockSwapper.h"

CBlockSwapper::CBlockSwapper()
    : model{g3d::resourceManager.get("selector_model")},
      boardOffset{0.f, 0.f, 0.f}, position{2, 10} {}

CBlockSwapper::~CBlockSwapper() {}

void CBlockSwapper::Control() { clamp(); }

void CBlockSwapper::setBoardOffset(const glm::vec3 &offset) {
    boardOffset = offset;
}

void CBlockSwapper::clamp() {
    position.y = std::clamp(position.y, 2, 12);
    position.x = std::clamp(position.x, 0, 4);
}

void CBlockSwapper::reset() { position = {2, 10}; }

void CBlockSwapper::setPosition(int x, int y) { position = {x, y}; }

void CBlockSwapper::draw(float scroll) {
    model->setPosition({position.x * 2.f + boardOffset.x,
                        25.f - 2.f * position.y + scroll * 2.f + boardOffset.y,
                       -10.f + boardOffset.z});
    model->draw();
}

void CBlockSwapper::moveUp() { position.y--; }

void CBlockSwapper::moveDown() { position.y++; }

void CBlockSwapper::moveLeft() { position.x--; }

void CBlockSwapper::moveRight() { position.x++; }

glm::ivec2 CBlockSwapper::getPositionL() { return position; }

glm::ivec2 CBlockSwapper::getPositionR() {
    return {position.x + 1, position.y};
}
