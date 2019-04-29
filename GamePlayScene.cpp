#include "GamePlayScene.h"



CGamePlayScene::CGamePlayScene(CGameBoard **board)
    : CInGameScene(board), lightAngle{0.f}, endGameTimer{0.f},
      winnerModel{g3d::resourceManager.get("winner_entry_model")},
      loserModel{g3d::resourceManager.get("loser_entry_model")},
      moveToNextScene{false} {
    winnerModel->setRotation({M_PI / 2.f, 0.f, 0.f});
    loserModel->setRotation({M_PI / 2.f, 0.f, 0.f});
    dirLight = g3d::DirectionalLight::create();
    dirLight->setAmbient({.3f, .3f, .3f});
    dirLight->setDiffuse({.2f, .2f, .2f});
    dirLight->setSpecular({.3f, .3f, .3f});
    dirLight->setDirection({0.f, -1.f, -1.f});
    dirLight->setActive(true);

    const glm::vec3 colors[]{{.95f, .3f, .3f}, {.2f, .2f, .8f},
                             {.3f, .2f, .8f},  {1.f, .0f, .0f},
                             {.1f, .8f, .2f},  {.9f, .7f, .3f}};

    for (int i = 0; i < 6; i++) {
        pointLights[i] = g3d::PointLight::create();
        pointLights[i]->setActive(true);
        pointLights[i]->setSpecular({.6f, .6f, .6f});
        pointLights[i]->setRange(12.f);
        pointLights[i]->setAmbient(colors[i]);
        pointLights[i]->setDiffuse(colors[i]);
    }

    pointLights[0]->setPosition({-2.f, 0.f, -8.f});
    pointLights[1]->setPosition({12.f, 0.f, -8.f});
    pointLights[2]->setPosition({16.f, 0.f, -8.f});
    pointLights[3]->setPosition({30.f, 0.f, -8.f});
    for (int i = 0; i < 4; i++) {
        auto pos = pointLights[i]->getPosition();
        pos.y = 12.f + sin(lightAngle + i * 2.f) * 11.f;
        pointLights[i]->setPosition(pos);
    }

    for (int i = 0; i < 2; i++) {
        pointLights[i + 4]->setPosition(
            {14.f + sin(.8f * lightAngle + i * M_PI) * 16.f, .2f, 0.f});
    }

    winnerModel->setPosition({10.2f, -5.f, 20.f});
    loserModel->setPosition({10.2f, -5.f, 20.f});
}

CGamePlayScene::~CGamePlayScene() {
    for (int i = 0; i < 6; i++)
        delete pointLights[i];
    delete dirLight;
}

void CGamePlayScene::draw() {
    CInGameScene::draw();

    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(.1f, .85f, 0.f));
    winnerModel->draw();
    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(1.f, .2f, .3f));
    loserModel->draw();
    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(1.f, 1.f, 1.f));
}

void CGamePlayScene::control(float deltaTime) {
    bool multiplayer{false};
    int playerCount{0};
    int loserCount{0};
    for (int i = 0; i < BOARD_MAX; i++) {
        if (boards[i]->isActive())
            playerCount++;
        if (boards[i]->lost())
            loserCount++;
        else
            boards[i]->control(deltaTime);
    }
    if (playerCount > 1)
        multiplayer = true;

    const glm::vec3 winLosePosition[]{{10.2f, 0.f, 20.f}, {17.75f, 0.f, 20.f}};

    if (playerCount == loserCount) {
        endGameTimer += deltaTime;

        if (multiplayer) {
            bool posID =
                boards[BOARD_1]->getScore() < boards[BOARD_2]->getScore();
            winnerModel->setPosition({winLosePosition[posID].x,
                                      -1.f + endGameTimer * 6.f,
                                      winLosePosition[posID].z});
            loserModel->setPosition({winLosePosition[!posID].x,
                                     -1.f + endGameTimer * 6.f,
                                     winLosePosition[!posID].z});
        }

        if (!moveToNextScene && endGameTimer > 4.f) {
            moveToNextScene = true;
            g3d::camera.setMoveAnimationLength(2.f);
            g3d::camera.setViewAnglesAnimationLength(2.f);
            g3d::camera.movePositionAnimation({14.f, 8.5f, 62.f});
            g3d::camera.moveAnglesAnimation({-60.f, 180.f, 0.f});
        }
    }

    lightAngle += deltaTime;
    for (int i = 0; i < 4; i++) {
        auto pos = pointLights[i]->getPosition();
        pos.y = 12.f + sin(.8f * lightAngle + i * 2.f) * 11.f;
        pointLights[i]->setPosition(pos);
    }
    for (int i = 0; i < 2; i++) {
        pointLights[i + 4]->setPosition(
            {14.f + sin(.4f * lightAngle + i * M_PI) * 16.f, .2f, -8.f});
    }
}

SCENE CGamePlayScene::getNextScene() {
    if (g3d::keyManager.isClicked(SDLK_ESCAPE))
        return SCENE::PAUSE_SCENE;

    if (moveToNextScene && !g3d::camera.isInAnimation()) {
        moveToNextScene = false;
        endGameTimer = 0.f;
        return SCENE::MAIN_MENU_SCENE;
    }
    return SCENE::GAMEPLAY_SCENE;
}

bool CGamePlayScene::quit() { return false; }
