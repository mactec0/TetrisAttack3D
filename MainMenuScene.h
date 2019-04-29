#pragma once
#include "InGameScene.h"
#include "ListMenu.h"

class CMainMenuScene final : public CInGameScene {
    CListMenu *menu;
    const glm::vec3 defaultPos, defaultViewAngles;
    g3d::PointLight *pointLights[2];
    g3d::DirectionalLight *dirLight;
    float lightAngle;
    bool moveToNextScene;
    bool quitGame;
    enum {
        ENTRY_SINGLE_PLAYER,
        ENTRY_MULTI_PLAYER,
        ENTRY_QUIT_PLAYER,
    };

  public:
    CMainMenuScene(CGameBoard **board);

    virtual ~CMainMenuScene();

    virtual void draw() override;

    virtual void control(float deltaTime) override;

    virtual SCENE getNextScene() override;

    virtual bool quit() override;
};
