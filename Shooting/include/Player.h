#pragma once
#include "Entity.h"
#include "ObjectPool.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>

class Player : public Entity {
public:
	Player(ObjectPool<Bullet, 128>& pool);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow & window) const override;

	void move(float deltaX, float deltaY);

private:
	float speed = 300.0f;
	mutable sf::RectangleShape shape;

	ObjectPool<Bullet, 128>& bulletPool;
	float shootCooldown = 1.0f;
};