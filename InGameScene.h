#pragma once
#include "GameBoard.h"
#include "Scene.h"

class CInGameScene : public IScene {
    g3d::Model *wall, *floor;

  protected:
    CInGameScene(CGameBoard **board);
    CGameBoard *boards[BOARD_MAX];

  public:
    virtual ~CInGameScene();

    virtual void draw() override;

    void drawGameScene();
};
