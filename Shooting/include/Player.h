#pragma once
#include "Entity.h"
#include "ObjectPool.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>

class Player : public Entity {
public:
	Player(ObjectPool<Bullet, 256>& pool);

	void update(float deltaTime) override;
	void draw(sf::RenderWindow & window) const override;

	void move(float deltaX, float deltaY);

private:
	float speed = 300.0f; //プレイヤーの移動速度
	mutable sf::RectangleShape shape;

	ObjectPool<Bullet, 256>& bulletPool;
	float shootCooldown = 1.0f;	//弾の発射間隔
};