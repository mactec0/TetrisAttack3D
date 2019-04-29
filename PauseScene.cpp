#include "PauseScene.h"

CPauseScene::CPauseScene(CGameBoard **board)
    : CInGameScene(board), menu{new CListMenu()},
      moveToNextScene{false}, nextScene{SCENE::GAMEPLAY_SCENE} {
    menu->setPosition({14.f, 12.f, 28.f});
    menu->setDirectionModifier({0.f, -1.f, 0.f});
    menu->addEntry(g3d::resourceManager.get("resume_entry_model"),
                   {M_PI / 2.f, 0.f, 0.f});
    menu->addEntry(g3d::resourceManager.get("mainmenu_entry_model"),
                   {M_PI / 2.f, 0.f, 0.f});
    menu->setSelectionColor({.88f, .25f, .25f});
    menu->setDefaultColor({.24f, .4f, .75f});
}

CPauseScene::~CPauseScene(){ delete menu; }

void CPauseScene::draw() {
    CInGameScene::draw();
    menu->draw();
}

void CPauseScene::control(float deltaTime) {
    menu->control();
    if (menu->IsEntryClicked()) {
        if (menu->getCurrentEntryID() == ENTRY_RESUME) {
            nextScene = SCENE::GAMEPLAY_SCENE;
        } else {
            nextScene = SCENE::MAIN_MENU_SCENE;
            g3d::camera.setMoveAnimationLength(2.f);
            g3d::camera.setViewAnglesAnimationLength(2.f);
            g3d::camera.movePositionAnimation({14.f, 8.5f, 62.f});
            g3d::camera.moveAnglesAnimation({-60.f, 180.f, 0.f});
        }
        moveToNextScene = true;
    }
}

SCENE CPauseScene::getNextScene() {
    if (moveToNextScene && !g3d::camera.isInAnimation()) {
        moveToNextScene = false;
        return nextScene;
    }
    return SCENE::PAUSE_SCENE;
}

bool CPauseScene::quit() { return false; }
