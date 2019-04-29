#pragma once

enum SCENE {
    MAIN_MENU_SCENE,
    DIFFICULTY_SCENE,
    GAMEPLAY_SCENE,
    PAUSE_SCENE,
    SCENE_MAX
};

class IScene {
public:
    virtual ~IScene(){};
    virtual void draw() = 0;
    virtual void control(float deltaTime) = 0;
    virtual SCENE getNextScene() = 0;
    virtual bool quit() = 0;
};
