#include "../include/Bullet.h"

Bullet::Bullet() {
	//弾の形状とサイズを設定
	shape.setSize({ 5.0f,15.0f });
	shape.setFillColor(sf::Color::Yellow);
}

void Bullet::update(float deltaTime) {
	//弾を上方向に移動
	position.y -= speed * deltaTime;
}

void Bullet::draw(sf::RenderWindow& window) const {
	//shapeの位置をプレイヤの位置(position)に合わせる
	shape.setPosition(sf::Vector2f(position.x, position.y));
	//弾を描画
	window.draw(shape);
}

void Bullet::init(float startX, float startY) {
	alive = true;
	position.x = startX;
	position.y = startY;
}