#include "GameBoard.h"

CGameBoard::CGameBoard(BOARD_ID ID)
    : scrollSpeed{0.1f}, scrollAcceleration{0.002f}, scroll{0.0f},
      scrollPause{0.f}, score{0}, active{false} {
    this->ID = ID;
    blockTransition.setBoardData(&boardData);

    const int keyCfgPlayer1[] = {SDLK_w, SDLK_s,     SDLK_a,
                                 SDLK_d, SDLK_LCTRL, SDLK_LSHIFT};
    const int keyCfgPlayer2[] = {SDLK_UP,    SDLK_DOWN, SDLK_LEFT,
                                 SDLK_RIGHT, SDLK_m,    SDLK_j};

    if (!ID)
        std::copy(keyCfgPlayer1, keyCfgPlayer1 + KEYCFG_MAX, keyCfg);
    else
        std::copy(keyCfgPlayer2, keyCfgPlayer2 + KEYCFG_MAX, keyCfg);

    blockSwapper.setBoardOffset({18.f * ID, 0.f, 0.f});
    numberDisplay.setPosition({ID ? 30.f : -2.f, 20.0f, -9.f});
    numberDisplay.setAlign(ID);
    numberDisplay.setScale(2.f);
}

CGameBoard::~CGameBoard() {}

void CGameBoard::control(float deltaTime) {
    if (!active)
        return;

    if (g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_NEWLINE]))
        addLine();


    if (scrollPause <= 0.f) {
        scroll += scrollSpeed * deltaTime;
        scrollSpeed += deltaTime * scrollAcceleration;
    } else {
        scrollPause -= deltaTime;
        if (scrollPause < 0.f)
            scrollPause = 0.f;
    }

    if (scroll > 1.f) {
        scroll -= 1.f;
        blockSwapper.moveUp();
        blockSwapper.clamp();
        blockTransition.moveUp();
        boardData.newLine();
    }

    blockTransition.Control(deltaTime);

    if (g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_UP]))
        blockSwapper.moveUp();
    if (g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_DOWN]))
        blockSwapper.moveDown();
    if (g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_LEFT]))
        blockSwapper.moveLeft();
    if (g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_RIGHT]))
        blockSwapper.moveRight();

    blockSwapper.Control();

    if (!blockTransition.isInTransition() &&
        g3d::keyManager.isClicked(keyCfg[KEYCFG::KEY_SWAP]) &&
        !boardData.isSetToRemove(blockSwapper.getPositionL()) &&
        !boardData.isSetToRemove(blockSwapper.getPositionR()) &&
        !boardData.isBlockFalling(blockSwapper.getPositionL()) &&
        !boardData.isBlockFalling(blockSwapper.getPositionR()) &&
        !boardData.isBlockFalling(blockSwapper.getPositionL() +
                                  glm::ivec2{0, -1}) &&
        !boardData.isBlockFalling(blockSwapper.getPositionR() +
                                  glm::ivec2{0, -1})) {
        blockTransition.start(blockSwapper.getPositionL(),
                              blockSwapper.getPositionR());
    }

    bool markToDelete[13][6] = {false};
    if (!blockTransition.isInTransition()) {
        for (int y = 0; y < 13; y++) {
            for (int x = 0; x < 4; x++) {
                if (!boardData.getBlockID({x, y}))
                    continue;
                if (boardData.isSetToRemove({x, y}) ||
                    boardData.isSetToRemove({x + 1, y}) ||
                    boardData.isSetToRemove({x + 2, y}))
                    continue;
                if (boardData.isBlockFalling({x, y}))
                    continue;

                if (boardData.getBlockID({x, y}) ==
                        boardData.getBlockID({x + 1, y}) &&
                    boardData.getBlockID({x + 1, y}) ==
                        boardData.getBlockID({x + 2, y})) {
                    markToDelete[y][x] = true;
                    markToDelete[y][x + 1] = true;
                    markToDelete[y][x + 2] = true;
                }
            }
        }
        for (int y = 0; y < 11; y++) {
            for (int x = 0; x < 6; x++) {
                if (!boardData.getBlockID({x, y}))
                    continue;
                if (boardData.isSetToRemove({x, y}) ||
                    boardData.isSetToRemove({x, y + 1}) ||
                    boardData.isSetToRemove({x, y + 2}))
                    continue;
                if (boardData.isBlockFalling({x, y}))
                    continue;

                if (boardData.getBlockID({x, y}) ==
                        boardData.getBlockID({x, y + 1}) &&
                    boardData.getBlockID({x, y + 1}) ==
                        boardData.getBlockID({x, y + 2})) {
                    markToDelete[y][x] = true;
                    markToDelete[y + 1][x] = true;
                    markToDelete[y + 2][x] = true;
                }
            }
        }

        int pauseCounter = 0;
        for (int y = 0; y < 13; y++) {
            for (int x = 0; x < 6; x++) {
                if (markToDelete[y][x]) {
                    boardData.setToRemove({x, y});
                    pauseCounter++;
                }
            }
        }
        if (pauseCounter > 0) {
            score += (int)(scrollPause + 1) * pauseCounter * 10;
            numberDisplay.setText(score);
        }
        scrollPause += pauseCounter * 0.5f;
    }

    boardData.updateBlink(deltaTime);
    boardData.updateGravity(deltaTime);
}

void CGameBoard::draw() {
    glm::vec3 vecOffset{18.f * ID, 0.f, 0.f};
    g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                glm::vec3(2.f, 2.f, 2.f));
    for (int y = 0; y < 14; y++) {
        if (y == 13)
            g3d::engine->getDefaultShader()->setUniform(
                "colorModulation", glm::vec3(.5f, .5f, .5f));
        if (y < 2)
            g3d::engine->getDefaultShader()->setUniform(
                "colorModulation", glm::vec3(2.f, 2.f, 2.f));

        for (int x = 0; x < 6; x++) {
            if (!boardData.getBlockID({x, y}))
                continue;

            box.setColor(boardData.getBlockColor({x, y}));

            if (blockTransition.InTransition({x, y})) {
                box.setPosition(blockTransition.getBlockPos({x, y}) +
                                vecOffset + glm::vec3(0, scroll * 2.f, 0));
                box.draw();
            } else if (boardData.isSetToRemove({x, y})) {
                GLfloat brightness{0.6f + 0.8f * sinf(6.f * boardData.getBlink({x, y}) * M_PI)};
                g3d::engine->getDefaultShader()->setUniform("colorModulation",
                    glm::vec3(brightness, brightness, brightness));
                box.setPosition({x * 2.f + vecOffset.x, 25.f + y * -2.f + scroll * 2.f, -10.f});
                box.draw();
                g3d::engine->getDefaultShader()->setUniform("colorModulation", glm::vec3(1.f, 1.f, 1.f));
            } else {
                box.setPosition({x * 2.f + vecOffset.x,
                                 25.f + y * -2.f + scroll * 2.f + boardData.getFallOffset({x, y}),
                                 -10.f});
                box.draw();
            }
        }
        g3d::engine->getDefaultShader()->setUniform("colorModulation",
                                                    glm::vec3(1.f, 1.f, 1.f));
    }

    if(active){
        blockSwapper.draw(scroll);
        numberDisplay.draw();
    }
}

bool CGameBoard::lost() {
    for (int x = 0; x < 6; x++)
        if (boardData.getBlockID({x, 0}))
            return true;
    return false;
}

void CGameBoard::addLine() {
    boardData.newLine();
    score += 50;
    numberDisplay.setText(score);
}

void CGameBoard::setActive(bool active) { this->active = active; }

bool CGameBoard::isActive() { return active; }

void CGameBoard::setup(uint32_t seed) {
    boardData.setSeed(seed);
    boardData.reset();
    blockSwapper.reset();
    score = 0;
    scroll = 0.0f;
    scrollPause = 0.f;
    numberDisplay.setText(0);
}

void CGameBoard::setDifficulty(int difficulty) {
    scrollSpeed=0.1f+0.015f*(float)difficulty;
}

int CGameBoard::getScore() { return score; }
