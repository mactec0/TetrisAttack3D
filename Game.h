#pragma once
#include "Engine.h"
#include "GameBoard.h"
#include "ListMenu.h"
#include "Scene.h"

#include "DifficultyScene.h"
#include "GamePlayScene.h"
#include "MainMenuScene.h"
#include "PauseScene.h"

class CGame {
    CGameBoard *boards[BOARD_MAX];
    IScene *scenes[SCENE_MAX];
    int currentScene;

  public:
    CGame();
    ~CGame();

    static CGame *create();

    void start();

    void renderFrame();
    void update(float deltaTime);
    void handleEvents();
};
