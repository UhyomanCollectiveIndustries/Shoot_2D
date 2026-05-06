#pragma once
#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <optional>

class GameOverState :public GameState {
public:
	GameOverState(int resultScore);
	std::unique_ptr<GameState>update(float dt, sf::RenderWindow& window)override;
	void draw(sf::RenderWindow& window)override;

private:
	int resultScore;
	sf::Font font;
	std::optional<sf::Text> gameOverText;
	std::optional<sf::Text> promptText;
};