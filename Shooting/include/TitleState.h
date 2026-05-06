#pragma once
#include "GameState.h"
#include <optional>

class TitleState :public GameState {
public:
	TitleState();
	
	std::unique_ptr<GameState> update(float dt, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;

private:
	sf::Font font;
	std::optional<sf::Text> titleText;
	std::optional<sf::Text> promptText;
};