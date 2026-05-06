#pragma once
#include "GameState.h"
#include "ObjectPool.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SoundPool.h"
#include "DebugWindow.h"
#include <SFML/Audio.hpp>
#include <optional>
#include <vector>

class PlayState :public GameState {
public:
	PlayState();

	std::unique_ptr<GameState> update(float dt, sf::RenderWindow& window) override;
	void draw(sf::RenderWindow& window) override;

private:
	//プール
	ObjectPool<Bullet, 128> bulletPool;
	ObjectPool<Enemy, 128>enemyPool;

	//プレイヤ
	std::optional<Player> player;

	//サウンド
	sf::Music bgm;
	sf::SoundBuffer shootBuf;
	sf::SoundBuffer explosionBuf;
	SoundPool<8> shootPool;
	SoundPool<8> explosionPool;

	//スポーン
	float spawnTimer = 0.f;

	//スコア
	int score = 0;
	sf::Font font;
	std::optional < sf::Text> scoreText;

	//デバックウィンドウ
	std::optional<DebugWindow> debugWindow;

	//デルタタイム計測
	sf::Clock clock;
};