#pragma once
#include "Entity.h"

class Enemy :public Entity {
public:
	Enemy();

	void update(float deltaTime)override;
	void draw(sf::RenderWindow& window)const override;

	void init(float startX, float startY);
private:
	float speed = 100.0f;	//“G‚Ì‘¬“x
	mutable sf::RectangleShape shape;	//“G‚ÌŒ©‚½–Ú
};