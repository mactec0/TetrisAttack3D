#pragma once
#include "Engine.h"
#include "GameBoardData.h"

class CBlockTransition {
    CGameBoardData *boardData;

    bool inTransition;

    // amount per second
    float animationSpeed;

    // transition value (0.f - 1.f)
    float transition;

    glm::ivec2 blockPositions[2];

    glm::vec3 rotateAroundOrigin(const glm::vec3 &startPos,
                                 const glm::vec3 &centerOffset,
                                 float transition);

public:
    CBlockTransition();
    ~CBlockTransition();

    void setBoardData(CGameBoardData *data);

    bool isInTransition();

    void start(const glm::ivec2 &blockL, const glm::ivec2 &blockR);

    void Control(float deltaTime);

    bool InTransition(const glm::ivec2 &block);

    void moveUp();

    glm::vec3 getBlockPos(const glm::ivec2 &block);
};
