#include "../include/Player.h"
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

Player::Player(ObjectPool<Bullet, 128>& pool) : bulletPool(pool) {
	position = { 400.0f,500.0f };	//初期位置
	//プレイヤーの形状とサイズを設定
	shape.setSize({ 40.0f,40.0f });
	shape.setFillColor(sf::Color::Blue);
}

void Player::update(float deltaTime) {
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	//入力に応じて移動量を計算
	//上
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		deltaY -= speed * deltaTime;
	}
	//下
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		deltaY += speed * deltaTime;
	}
	//左
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
		deltaX -= speed * deltaTime;
	}
	//右
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
		deltaX += speed * deltaTime;
	}

	//スペースキーで弾を発射
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (shootCooldown <= 0.0f) {
			Bullet* bullet = bulletPool.alloc();
			if (bullet) {
				bullet->init(position.x + 17.5f, position.y);
				shootCooldown = 0.3f; //0.3秒クールダウン
			}
		}
	}
	shootCooldown -= deltaTime;

	//適応
	move(deltaX, deltaY);
}

void Player::draw(sf::RenderWindow& window) const {
	//shapeの位置をプレイヤの位置(position)に合わせる
	shape.setPosition(sf::Vector2f(position.x, position.y));
	//プレイヤーを描画
	window.draw(shape);
}

void Player::move(float deltaX, float deltaY) {
	position.x += deltaX;
	position.y += deltaY;
}