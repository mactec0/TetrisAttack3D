#include "GameBoardData.h"

CGameBoardData::CGameBoardData() : bottomID{16}, seed{31337} { reset(); }

CGameBoardData::~CGameBoardData() {}

uint32_t CGameBoardData::getSeed() { return seed; }

void CGameBoardData::setSeed(uint32_t s) { seed = s; }

void CGameBoardData::newLine() {
    bottomID = normalizeY(bottomID + 1);
    genLine(bottomID);
}

void CGameBoardData::genOneLine(int y) {
    int x{2};
    boardBuffer[y][0] = generator.rand(1, 5);
    boardBuffer[y][1] = generator.rand(1, 5);

    while (x < 6) {
        auto next_value = generator.rand(1, 5);
        if (boardBuffer[y][x - 2] != boardBuffer[y][x - 1] ||
            boardBuffer[y][x - 1] != next_value) {
            boardBuffer[y][x] = next_value;
            x++;
        }
    }
}

void CGameBoardData::genLine(int y) {
    int x{2};
    for (int i = 0; i < 2; i++) {
        int next_value{generator.rand(1, 5)};
        while (boardBuffer[normalizeY(y - 2)][i] == boardBuffer[normalizeY(y - 1)][i] &&
               boardBuffer[normalizeY(y - 1)][i] == next_value)
            next_value = generator.rand(1, 5);
        boardBuffer[y][i] = next_value;
        fpBuffer[y][i] = 0.0;
    }
    while (x < 6) {
        int next_value{generator.rand(1, 5)};
        if ((boardBuffer[y][x - 2] != boardBuffer[y][x - 1] ||
             boardBuffer[y][x - 1] != next_value) &&
            (boardBuffer[normalizeY(y - 2)][x] != boardBuffer[normalizeY(y - 1)][x] ||
             boardBuffer[normalizeY(y - 1)][x] != next_value)) {
            boardBuffer[y][x] = next_value;
            fpBuffer[y][x] = 0.0;
            x++;
        }
    }
}

void CGameBoardData::genBegin() {
    generator.setSeed(seed);
    genOneLine(13);
    genOneLine(14);
    genLine(15);
    genLine(16);
}

void CGameBoardData::reset() {
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 6; x++) {
            boardBuffer[y][x] = 0.0;
            fpBuffer[y][x] = 0.0;
        }
    }
    bottomID = 16;
    genBegin();
}

int CGameBoardData::normalizeY(int y) {
    while (y >= 17)
        y -= 17;
    while (y < 0)
        y += 17;
    return y;
}

bool CGameBoardData::isLineEmpty(int y) {
    int v = 0;
    for (int x = 0; x < 6; x++)
        v += boardBuffer[y][x];
    return v == 0;
}

void CGameBoardData::clearTop() {
    int y{normalizeY(bottomID + 1)};
    if (isLineEmpty(y)) {
        while (isLineEmpty(y))
            y = normalizeY(y + 1);
    }

    for (int x = 0; x < 6; x++)
        boardBuffer[y][x] = 0x00;
}

uint8_t CGameBoardData::getBlockID(const glm::ivec2 &block) {
    int correctY{bottomID + 1};
    return boardBuffer[normalizeY(correctY + block.y)][block.x];
}

BOX_COLOR CGameBoardData::getBlockColor(const glm::ivec2 &block) {
    auto id{getBlockID(block)};
    return (id >= BOX_COLOR::BOX_FALLING)
               ? (BOX_COLOR)(id - BOX_COLOR::BOX_FALLING - 1)
               : (BOX_COLOR)(id - 1);
}

void CGameBoardData::set(const glm::ivec2 &block, uint8_t value) {
    int correctY{bottomID + 1};
    boardBuffer[normalizeY(correctY + block.y)][block.x] = value;
}

bool CGameBoardData::isBlockFalling(const glm::ivec2 &block) {
    return (getBlockID(block) >= BOX_COLOR::BOX_FALLING) ? true : false;
}

bool CGameBoardData::isSetToRemove(const glm::ivec2 &block) {
    int correctY = bottomID + 1;
    return (fpBuffer[normalizeY(correctY + block.y)][block.x] > 0.0);
}

void CGameBoardData::setToRemove(const glm::ivec2 &block) {
    int correctY = bottomID + 1;
    fpBuffer[normalizeY(correctY + block.y)][block.x] = 1.0;
}

double CGameBoardData::getBlink(const glm::ivec2 &block) {
    int correctY = bottomID + 1;
    return fpBuffer[normalizeY(correctY + block.y)][block.x];
}

double CGameBoardData::getFallOffset(const glm::ivec2 &block) {
    int correctY = bottomID + 1;
    return fpBuffer[normalizeY(correctY + block.y)][block.x];
}

void CGameBoardData::updateBlink(double dtime) {
    int correctY{bottomID + 1};
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 6; x++) {
            if (isBlockFalling({x, y}))
                continue;
            if (fpBuffer[normalizeY(correctY + y)][x] > 0.0) {
                fpBuffer[normalizeY(correctY + y)][x] -= dtime;
                if (fpBuffer[normalizeY(correctY + y)][x] <= 0.0) {
                    boardBuffer[normalizeY(correctY + y)][x] = 0;
                    fpBuffer[normalizeY(correctY + y)][x] = 0.0;
                }
            }
        }
    }
}

void CGameBoardData::updateGravity(double dtime) {
    const double fallSpeed{64.0};
    int correctY{bottomID + 1};
    for (int y = 13; y > 0; y--) {
        for (int x = 0; x < 6; x++) {
            auto underBlock{normalizeY(correctY + y)};
            auto currentBlock{normalizeY(correctY + y - 1)};
            if (isSetToRemove({x, y - 1}))
                continue;
            if ((boardBuffer[underBlock][x] == 0 ||
                 boardBuffer[underBlock][x] >= BOX_COLOR::BOX_FALLING) &&
                 boardBuffer[currentBlock][x] != 0 && y < 13) {

                if (boardBuffer[currentBlock][x] < BOX_COLOR::BOX_FALLING) {
                    boardBuffer[currentBlock][x] += BOX_COLOR::BOX_FALLING;
                    fpBuffer[currentBlock][x] = 0.0;
                } else {
                    fpBuffer[currentBlock][x] -= fallSpeed * dtime;
                    if (fpBuffer[currentBlock][x] <= -2.0 &&
                        boardBuffer[underBlock][x] == 0) {
                        boardBuffer[underBlock][x] = boardBuffer[currentBlock][x];
                        boardBuffer[currentBlock][x] = 0;
                        fpBuffer[underBlock][x] = fpBuffer[currentBlock][x] + 2.0;
                        fpBuffer[currentBlock][x] = 0.0;
                    }
                }
            } else if (boardBuffer[underBlock][x] != 0 &&
                       boardBuffer[currentBlock][x] >= BOX_COLOR::BOX_FALLING) {
                boardBuffer[currentBlock][x] -= BOX_COLOR::BOX_FALLING;
                fpBuffer[currentBlock][x] = 0.0;
            }
        }
    }
}
