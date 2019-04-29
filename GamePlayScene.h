#pragma once
#include "InGameScene.h"

class CGamePlayScene final : public CInGameScene {
    g3d::DirectionalLight *dirLight;
    g3d::PointLight *pointLights[6];
    float lightAngle;
    float endGameTimer;
    g3d::Model *winnerModel, *loserModel;
    bool moveToNextScene;

 public:
    CGamePlayScene(CGameBoard **board);

    virtual ~CGamePlayScene();

    virtual void draw() override;

    virtual void control(float deltaTime) override;

    virtual SCENE getNextScene() override;

    virtual bool quit() override;
};
