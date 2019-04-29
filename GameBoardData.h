#pragma once
#include "Cube.h"
#include "Engine.h"
#include "RandomGenerator.h"
#include <stdint.h>

class CGameBoardData {
    RandomGenerator generator;
    uint8_t boardBuffer[17][6];
    double fpBuffer[17][6];

    int bottomID;
    uint32_t seed;

    void genOneLine(int y);
    void genLine(int y);
    void genBegin();

    int normalizeY(int y);

    bool isLineEmpty(int y);

 public:
    CGameBoardData();
    ~CGameBoardData();

    void reset();

    void clearTop();

    void newLine();

    uint32_t getSeed();
    void setSeed(uint32_t s);

    uint8_t getBlockID(const glm::ivec2 &block);

    BOX_COLOR getBlockColor(const glm::ivec2 &block);

    void swap(glm::ivec2 v1, glm::ivec2 v2) {
        int correctY = bottomID + 1;
        auto y = normalizeY(correctY + v1.y);
        auto temp = boardBuffer[y][v1.x];
        boardBuffer[y][v1.x] = boardBuffer[y][v2.x];
        boardBuffer[y][v2.x] = temp;
    }

    void set(const glm::ivec2 &block, uint8_t value);

    bool isBlockFalling(const glm::ivec2 &block);

    bool isSetToRemove(const glm::ivec2 &block);
    void setToRemove(const glm::ivec2 &block);
    double getBlink(const glm::ivec2 &block);
    double getFallOffset(const glm::ivec2 &block);
    void updateBlink(double dtime);
    void updateGravity(double dtime);
};
