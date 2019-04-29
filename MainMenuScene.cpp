#include "MainMenuScene.h"


CMainMenuScene::CMainMenuScene(CGameBoard **board)
: CInGameScene(board), menu{new CListMenu()},
  defaultPos{14.f, 8.5f, 62.f}, defaultViewAngles{-60.f, 180.f, 0.f},
  lightAngle{0.f}, moveToNextScene{false}, quitGame{false} {
    g3d::camera.setPosition(defaultPos);
    g3d::camera.setViewAngles(defaultViewAngles);
    menu->setPosition({14.f,2.f,57.5f});
    menu->setDirectionModifier({0.f, 0.f, 1.f});
    menu->addEntry(g3d::resourceManager.get("singleplayer_entry_model"),{0.f,0.f,0.f});
    menu->addEntry(g3d::resourceManager.get("multiplayer_entry_model"),{0.f,0.f,0.f});
    menu->addEntry(g3d::resourceManager.get("quit_entry_model"),{0.f,0.f,0.f});
    menu->setSelectionColor({.88f, .25f, .25f});
    menu->setDefaultColor({.24f, .4f, .75f});

    dirLight = g3d::DirectionalLight::create();
    dirLight->setAmbient({0.1f, 0.1f, 0.1f});
    dirLight->setDiffuse({0.1f, 0.1f, 0.1f});
    dirLight->setSpecular({.18f, .18f, .18f});
    dirLight->setDirection({0.f, -1.f, 0.f});
    dirLight->setActive(true);

    for (int i = 0; i < 2; i++) {
        pointLights[i] = g3d::PointLight::create();
        pointLights[i]->setActive(true);
        pointLights[i]->setPosition({14.f, 1.f, 56.f});
        pointLights[i]->setSpecular({.8f, .8f, .8f});
        pointLights[i]->setRange(8.f);
    }

    pointLights[0]->setAmbient({.25f, .85f, .95f});
    pointLights[0]->setDiffuse({.25f, .9f, 1.f});
    pointLights[1]->setAmbient({.8f, .2f, .8f});
    pointLights[1]->setDiffuse({.9f, .2f, .9f});

}

CMainMenuScene::~CMainMenuScene() {
    for (int i = 0; i < 2; i++)
        delete pointLights[i];
    delete dirLight;
    delete menu;
}

void CMainMenuScene::draw() {
    CInGameScene::draw();
    menu->draw();

    if (menu->IsEntryClicked()) {
        if (menu->getCurrentEntryID() == ENTRY_QUIT_PLAYER) {
            quitGame = true;
        } else {
            if (menu->getCurrentEntryID() == ENTRY_SINGLE_PLAYER) {
                boards[BOARD_1]->setActive(true);
                boards[BOARD_2]->setActive(false);
            } else {
                boards[BOARD_1]->setActive(true);
                boards[BOARD_2]->setActive(true);
            }
            auto seed = time(NULL);
            for (int i = 0; i < BOARD_MAX; i++)
                boards[i]->setup(seed);
            g3d::camera.setMoveAnimationLength(1.f);
            g3d::camera.setViewAnglesAnimationLength(1.f);
            g3d::camera.movePositionAnimation({14.f, 12.f, 48.f});
            g3d::camera.moveAnglesAnimation({-45.f, 180.f, 0.f});

            moveToNextScene = true;
        }
    }
}

void CMainMenuScene::control(float deltaTime) {
    menu->control();
    lightAngle += deltaTime;
    for (int i = 0; i < 2; i++) {
        pointLights[i]->setPosition(
            {14.f + sin(1.5f * lightAngle + i * M_PI) * 4.f, 1.f,
             56.f + menu->getCurrentEntryID()});
    }
}

SCENE CMainMenuScene::getNextScene() {
    if (moveToNextScene && !g3d::camera.isInAnimation()) {
        moveToNextScene = false;
        return SCENE::DIFFICULTY_SCENE;
    }
    return SCENE::MAIN_MENU_SCENE;
}

bool CMainMenuScene::quit() { return quitGame; }
