#pragma once
#include "Entity.h"

class Bullet : public Entity {
public:
	Bullet();

	void update(float del) override;
	void draw(sf::RenderWindow& window) const override;

	void init(float startX, float startY);

private:
	float speed = 500.0f;	//’e‚Ì‘¬“x
	mutable sf::RectangleShape shape;	//’e‚ÌŒ©‚½–Ú
};