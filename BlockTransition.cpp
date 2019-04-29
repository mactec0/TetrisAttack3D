#include "BlockTransition.h"

CBlockTransition::CBlockTransition()
    : boardData{nullptr}, inTransition{false}, animationSpeed{8.f},
      transition{0.f}, blockPositions{{0, 0}, {0, 0}} {}

CBlockTransition::~CBlockTransition(){ }

void CBlockTransition::setBoardData(CGameBoardData *data) { boardData = data; }

bool CBlockTransition::isInTransition() { return inTransition; }

void CBlockTransition::start(const glm::ivec2 &blockL,
                             const glm::ivec2 &blockR) {
    inTransition = true;
    transition = 0.f;
    blockPositions[0] = blockL;
    blockPositions[1] = blockR;
}


void CBlockTransition::Control(float deltaTime) {
    if (!inTransition)
        return;
    transition += deltaTime * animationSpeed;
    if (transition > 1.f) {
        if (boardData)
            boardData->swap(blockPositions[0], blockPositions[1]);
        inTransition = false;
    }
}


bool CBlockTransition::InTransition(const glm::ivec2 &block) {
    if (!inTransition)
        return false;
    return (block == blockPositions[0] || block == blockPositions[1]);
}

glm::vec3 CBlockTransition::rotateAroundOrigin(const glm::vec3 &startPos,
                                               const glm::vec3 &centerOffset,
                                               float transition) {
    glm::vec4 rotationPoint{startPos,1.f};
    glm::vec4 centerPoint{startPos, 1.f};
    centerPoint+=glm::vec4(centerOffset,0.f);
    glm::mat4x4 rotationMatrix{ glm::rotate(glm::pi<float>()*transition, glm::vec3{0.0f, 1.0f, 0.0f}) };
    glm::mat4x4 translationMatrix{ glm::translate(glm::mat4(), glm::vec3{centerPoint}) };
    glm::mat4x4 finalTransformationMatrix{ translationMatrix * rotationMatrix * glm::inverse(translationMatrix) };
    return glm::vec3(finalTransformationMatrix * rotationPoint);
}

void CBlockTransition::moveUp() {
    if (!inTransition)
        return;
    blockPositions[0].y--;
    blockPositions[1].y--;
}

glm::vec3 CBlockTransition::getBlockPos(const glm::ivec2 &block) {
    if(blockPositions[0]==block){
        auto startPos = glm::vec3(blockPositions[0].x*2.f, 25.f-2.f*blockPositions[0].y, -10.f);
        return rotateAroundOrigin(startPos, {1.f, 0.f, 0.f}, transition);
    }else if(blockPositions[1]==block){
        auto startPos = glm::vec3(blockPositions[0].x*2.f, 25.f-2.f*blockPositions[0].y, -10.f);
        return rotateAroundOrigin(startPos, {1.f, 0.f, 0.f}, transition+1.f);
    }
    return {0, 0, 0};
}
