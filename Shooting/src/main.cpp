#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../include/GameState.h"
#include "../include/TitleState.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //乱数
    srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 800,600 }), "Shoot_2D");
    sf::Clock clock;

    //タイトルシーン
    std::unique_ptr<GameState> currentState = std::make_unique<TitleState>();

    while (window.isOpen()) {
        //デルタタイム計測
        float dt = clock.restart().asSeconds();

        //更新
        auto nextState = currentState->update(dt, window);
        if (nextState) {
            currentState->onExit();
            currentState = std::move(nextState);
            currentState->onEnter();
        }

        window.clear(sf::Color::Black);
        currentState->draw(window);
        window.display();
    }

    return 0;
}