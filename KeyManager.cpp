#include "KeyManager.h"

namespace g3d {

CKeyManager::CKeyManager() {}

CKeyManager::~CKeyManager() {}

bool CKeyManager::isClicked(int k) { return clicked[k]; }

bool CKeyManager::isPressed(int k) { return pressed[k]; }

void CKeyManager::setClicked(int k, bool v) { clicked[k] = v; }

void CKeyManager::setPressed(int k, bool v) { pressed[k] = v; }

void CKeyManager::resetClicked() {
    for (auto &[key, value] : clicked)
        value = false;
}

};
