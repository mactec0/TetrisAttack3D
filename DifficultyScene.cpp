#include "DifficultyScene.h"

CDifficultyScene::CDifficultyScene(CGameBoard **board)
    : CInGameScene(board), difficultyLvl{1}, moveToNextScene{false},
      difficultyText{g3d::resourceManager.get("difficulty_entry_model")} {
    numberDisplay.setPosition({13.3f, 8.f, 45.f});
    numberDisplay.setColor({0.f, 1.f, 0.f});
    numberDisplay.setRotation({M_PI / 4.f, 0.f, 0.f});
    numberDisplay.setText(difficultyLvl);

    difficultyText->setPosition({11.25f, 8.f, 43.f});
    difficultyText->setRotation({M_PI / 4.f, 0.f, 0.f});
}

CDifficultyScene::~CDifficultyScene() {}

void CDifficultyScene::draw() {
    CInGameScene::draw();
    numberDisplay.draw();

    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(.3f, .85f, .9f));
    difficultyText->draw();
    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(1.f, 1.f, 1.f));
}

void CDifficultyScene::control(float deltaTime) {
    if (g3d::keyManager.isClicked(SDLK_LEFT))
        difficultyLvl--;
    else if (g3d::keyManager.isClicked(SDLK_RIGHT))
        difficultyLvl++;

    difficultyLvl = std::clamp(difficultyLvl, 1, 100);
    numberDisplay.setText(difficultyLvl);

    auto difficultyToColor = [](int H) -> glm::vec3 {
        const double S{1.f};
        const double V{1.f};
        H = 101 - H;
        double C{S * V};
        double X{C * (1 - abs(fmod(H / 60.0, 2) - 1))};
        double m{V - C};
        double Rs{0.0}, Gs{0.0}, Bs{0.0};
        if (H >= 0 && H < 60) {
            Rs = C;
            Gs = X;
            Bs = 0;
        } else if (H >= 60 && H < 120) {
            Rs = X;
            Gs = C;
            Bs = 0;
        }
        return {Rs + m, Gs + m, Bs + m};
    };

    numberDisplay.setColor(difficultyToColor(difficultyLvl));

    if (g3d::keyManager.isClicked(SDLK_RETURN)) {
        for (int i = 0; i < BOARD_MAX; i++)
            boards[i]->setDifficulty(difficultyLvl);
        moveToNextScene = true;
        g3d::camera.setMoveAnimationLength(1.f);
        g3d::camera.setViewAnglesAnimationLength(1.f);
        g3d::camera.movePositionAnimation({14.f, 12.f, 42.f});
        g3d::camera.moveAnglesAnimation({0.f, 180.f, 0.f});
    }
}

SCENE CDifficultyScene::getNextScene() {
    if (moveToNextScene && !g3d::camera.isInAnimation()) {
        moveToNextScene = false;
        return SCENE::GAMEPLAY_SCENE;
    }
    return SCENE::DIFFICULTY_SCENE;
}


bool CDifficultyScene::quit(){ return false; }
