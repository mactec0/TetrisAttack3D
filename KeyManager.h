#pragma once
#include <map>
#include <unordered_map>

namespace g3d {

class CKeyManager {
    std::unordered_map<int, bool> clicked;
    std::unordered_map<int, bool> pressed;

  public:
    CKeyManager();
    ~CKeyManager();

    bool isClicked(int k);
    bool isPressed(int k);

    void setClicked(int k, bool v);
    void setPressed(int k, bool v);

    void resetClicked();
};

extern CKeyManager keyManager;

};
