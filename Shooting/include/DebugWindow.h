#pragma once
#include <SFML/Graphics.hpp>
#include "../include/ObjectPool.h"
#include "../include/Bullet.h"
#include "../include/Enemy.h"
#include <optional>

class DebugWindow {
public:
	DebugWindow(
		ObjectPool<Bullet, 128>& bPool,
		ObjectPool<Enemy, 128>& ePool
		);

	void toggleVisible();
	bool isVisible() const;

	void update();
	void render();

private:
	sf::RenderWindow window;
	sf::Font font;
	std::optional<sf::Text> bulletText;
	std::optional<sf::Text> enemyText;

	ObjectPool<Bullet, 128>& bPool;
	ObjectPool<Enemy, 128>& ePool;

	bool visible = false;
};