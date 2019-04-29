#pragma once
#include "InGameScene.h"
#include "ListMenu.h"

class CPauseScene final : public CInGameScene {
    CListMenu *menu;
    bool moveToNextScene;
    SCENE nextScene;
    enum { ENTRY_RESUME, ENTRY_MAINMENU };

public:
    CPauseScene(CGameBoard **board);

    virtual ~CPauseScene();

    virtual void draw() override;

    virtual void control(float deltaTime) override;

    virtual SCENE getNextScene() override;

    virtual bool quit() override;
};
