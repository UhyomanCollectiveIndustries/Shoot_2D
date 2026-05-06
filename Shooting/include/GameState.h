#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class GameState {
public:
    virtual ~GameState() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    //çXêV
    virtual std::unique_ptr<GameState> update(float dt,sf::RenderWindow& window) = 0;
    //ï`âÊ
    virtual void draw(sf::RenderWindow& window) = 0;
};