#pragma once
#include "BlockSwapper.h"
#include "BlockTransition.h"
#include "Cube.h"
#include "Engine.h"
#include "GameBoardData.h"
#include "NumberDisplay.h"

enum BOARD_ID { BOARD_1, BOARD_2, BOARD_MAX };

class CGameBoard {
    enum KEYCFG {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_NEWLINE,
        KEY_SWAP,
        KEYCFG_MAX
    };

    CCube box;
    CBlockSwapper blockSwapper;
    CBlockTransition blockTransition;
    CNumberDisplay numberDisplay;

    CGameBoardData boardData;
    BOARD_ID ID;

    int keyCfg[KEYCFG_MAX];

    // scroll amount per second
    float scrollSpeed;

    // scrollAcceleration per second;
    float scrollAcceleration;

    // scroll value (0.f - 1.f)
    float scroll;

    float scrollPause;

    int score;

    bool active;

    void setColor(int color);

  public:
    CGameBoard(BOARD_ID ID);
    ~CGameBoard();

    void draw();

    bool lost();

    void addLine();

    void setActive(bool active);

    bool isActive();

    void control(float deltaTime);

    void setup(uint32_t seed);

    void setDifficulty(int difficulty);

    int getScore();
};
