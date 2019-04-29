#include "InGameScene.h"

CInGameScene::CInGameScene(CGameBoard **board)
    : wall{g3d::resourceManager.get("wall_model")},
      floor{g3d::resourceManager.get("floor_model")},
      boards{board[BOARD_1], board[BOARD_2]} {}

CInGameScene::~CInGameScene() {}

void CInGameScene::draw() {
    floor->draw();
    for (int i = 0; i < 2; i++) {
        if (!boards[i]->isActive())
            g3d::engine->getDefaultShader()->setUniform(
                "colorModulation", glm::vec3(.2f, .2f, .2f));

        wall->setPosition({18.f * i - 1.f, 0.0f, -10.0f});
        wall->draw();
        g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                    glm::vec3(1.f, 1.f, 1.f));
        boards[i]->draw();
    }
}

void CInGameScene::drawGameScene() {}
