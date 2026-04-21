#pragma once
#include <SFML/Graphics.hpp>

//座標
struct Vec2 {
	float x, y;
};

class Entity {
public:
	//デストラクタ
	virtual ~Entity() = default;

	//更新
	virtual void update(float deltaTime) = 0;

	//描画
	virtual void draw(sf::RenderWindow& window) const = 0;

	Vec2 getPosition() const { return position; }

	//生存状態の取得と破壊
	bool isAlive() const { return alive; }
	void destroy() { alive = false; }

protected:
	//座標
	Vec2 position{ 0.f,0.f };

	//生存状態
	bool alive = true;
};