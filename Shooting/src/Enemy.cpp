#include "../include/Enemy.h"

Enemy::Enemy() {
	//“G‚ÌƒTƒCƒY‚ÆF‚ğİ’è
	shape.setSize({ 40.f,40.f });
	shape.setFillColor(sf::Color::Red);
}

void Enemy::update(float deltaTime) {
	//“G‚ğ‰º•ûŒü‚ÉˆÚ“®
	position.y += speed * deltaTime;
}

void Enemy::draw(sf::RenderWindow& window)const {
	//“G‚ÌˆÊ’u‚ğİ’è
	shape.setPosition(sf::Vector2f(position.x, position.y));
	//“G‚ğ•`‰æ
	window.draw(shape);
}

void Enemy::init(float startX, float startY) {
	alive = true;
	position.x = startX;
	position.y = startY;
}