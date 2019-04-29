#include "Camera.h"

namespace g3d {
Camera::Camera()
    : fov(30.f), position{0.0f, 0.0f, 0.0f}, targetPos{0.0f, 0.0f, 0.0f},
      vecViewAngles{0.0f, glm::pi<float>(), 0.0f}, moveAnimationLength(1.f),
      viewAnglesAnimationLength(1.f), inAnglesAnimation(false),
      inMoveAnimation(false) {
    update();
}

void Camera::update() {
    look = glm::vec3(cosf(vecViewAngles.x) * sinf(vecViewAngles.y),
                     sinf(vecViewAngles.x),
                     cosf(vecViewAngles.x) * cosf(vecViewAngles.y));

    look = glm::normalize(look);

    vecRight = glm::normalize(glm::cross(look, glm::vec3(0.0f, 1.0f, 0.0f)));
    vecUP = glm::normalize(glm::cross(vecRight, look));

    targetPos = position + look;
    viewMatrix = glm::lookAt(position, targetPos, vecUP);
}

void Camera::normalizeAngles(glm::vec3 &angles) {
    angles.x = glm::clamp(angles.x, -glm::pi<float>() / 2.0f + 0.1f,
                          glm::pi<float>() / 2.0f - 0.1f);

    while (angles.y > glm::pi<float>())
        angles.y -= 2.f * glm::pi<float>();

    while (angles.y < -glm::pi<float>())
        angles.y += 2.f * glm::pi<float>();
}

glm::mat4 Camera::getViewMatrix() { return viewMatrix; }

glm::vec3 Camera::getUp() { return vecUP; }

glm::vec3 Camera::getRight() { return vecRight; }

glm::vec3 Camera::getLook() { return look; }

glm::vec3 Camera::getPosition() { return position; }

float Camera::getFov() { return fov; }

void Camera::setFov(float fov) { this->fov = fov; }

void Camera::setPosition(const glm::vec3 &position) {
    this->position = position;
    update();
}

void Camera::rotate(const glm::vec3 &viewAngles) {
    vecViewAngles.y += glm::radians(viewAngles.y);
    vecViewAngles.x += glm::radians(viewAngles.x);

    normalizeAngles(vecViewAngles);

    update();
}

void Camera::move(const glm::vec3 &offset) {
    position += offset;
    update();
}

glm::vec3 Camera::getViewAngles() {
    return {glm::degrees(vecViewAngles.x),
            glm::degrees(vecViewAngles.y),
            glm::degrees(vecViewAngles.z)};
}

void Camera::setViewAngles(const glm::vec3 &viewAngles) {
    vecViewAngles ={glm::radians(viewAngles.x),
                    glm::radians(viewAngles.y),
                    glm::radians(viewAngles.z)};
}

void Camera::setMoveAnimationLength(float length) {
    moveAnimationLength = length;
}

void Camera::setViewAnglesAnimationLength(float length) {
    viewAnglesAnimationLength = length;
}

void Camera::movePositionAnimation(const glm::vec3 &finalPos) {
    targetAnimationPosition = finalPos;
    inMoveAnimation = true;
    positionAnimationStart = position;
    moveAnimationPercentage = 0.f;
}

void Camera::moveAnglesAnimation(const glm::vec3 &finalAngles) {
    targetAnimationAngles = {glm::radians(finalAngles.x),
                             glm::radians(finalAngles.y),
                             glm::radians(finalAngles.z)};
    normalizeAngles(targetAnimationAngles);
    inAnglesAnimation = true;
    anglesAnimationStart = vecViewAngles;
    anglesAnimationPercentage = 0.f;
}

bool Camera::isInAnimation() { return (inAnglesAnimation || inMoveAnimation); }


void Camera::updateAnimation(float deltaTime) {
    auto lerp = [](const glm::vec3 &A, const glm::vec3 &B,
                   float p) -> glm::vec3 { return (A + p * (B - A));
    };

    auto lerpAngles = [](const glm::vec3 &A, const glm::vec3 &B,
                         float p) -> glm::vec3 {
        glm::vec3 diff = B - A;
        while (diff.y > glm::pi<float>())
            diff.y -= 2.f * glm::pi<float>();
        while (diff.y < -glm::pi<float>())
            diff.y += 2.f * glm::pi<float>();
        return (A + p * diff);
    };

    if (inMoveAnimation) {
        moveAnimationPercentage += deltaTime / moveAnimationLength;
        position = lerp(positionAnimationStart, targetAnimationPosition,
                        moveAnimationPercentage);

        if (moveAnimationPercentage >= 1.f) {
            position = targetAnimationPosition;
            inMoveAnimation = false;
        }
    }

    if (inAnglesAnimation) {
        anglesAnimationPercentage += deltaTime / viewAnglesAnimationLength;
        vecViewAngles = lerpAngles(anglesAnimationStart, targetAnimationAngles,
                                   anglesAnimationPercentage);

        if (anglesAnimationPercentage >= 1.f) {
            vecViewAngles = targetAnimationAngles;
            inAnglesAnimation = false;
        }
    }

    update();
}

}; 
