#pragma once
#include "InGameScene.h"
#include "NumberDisplay.h"
#include <cmath>


class CDifficultyScene final : public CInGameScene {
    CNumberDisplay numberDisplay;
    int difficultyLvl;
    bool moveToNextScene;
    g3d::Model *difficultyText;

 public:
    CDifficultyScene(CGameBoard **board);

    virtual ~CDifficultyScene();

    virtual void draw() override;

    virtual void control(float deltaTime) override;

    virtual SCENE getNextScene() override;

    virtual bool quit() override;
};
