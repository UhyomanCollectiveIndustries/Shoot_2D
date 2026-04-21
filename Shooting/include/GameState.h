#pragma once

class GameState {
public:
    virtual ~GameState() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    //XV
    virtual void update(float dt) = 0;
    //•`‰æ
    virtual void draw() const = 0;
};