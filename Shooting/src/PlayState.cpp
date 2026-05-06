#include "../include/PlayState.h"
#include"../include/GameOverState.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

PlayState::PlayState() {
	//BGMのロード
	if (!bgm.openFromFile("assets/sounds/maou_game_vehicle01.ogg")) {
		std::cerr << "BGM load failed" << std::endl;
	}
	bgm.setLooping(true);
	bgm.setVolume(50.f);
	bgm.play();

	//SEのロード
	if (!shootBuf.loadFromFile("assets/sounds/maou_se_system42.ogg")) {
		std::cerr << "SE1 load failed" << std::endl;
	}

	if (!explosionBuf.loadFromFile("assets/sounds/maou_se_battle09.ogg")) {
		std::cerr << "SE2 load failed" << std::endl;
	}

	//PoolにSEをセット
	shootPool.setBuffer(shootBuf);
	explosionPool.setBuffer(explosionBuf);

	//Playerの初期化
	player.emplace(bulletPool, shootPool);

	//テキストの初期化
	if (!font.openFromFile("assets/fonts/Orbitron-Black.ttf")) {
		std::cerr << "Font load failed" << std::endl;
	}
	scoreText.emplace(font);
	scoreText->setCharacterSize(24);
	scoreText->setFillColor(sf::Color::White);
	scoreText->setPosition({ 10.f,10.f });

	//デバックウィンドウの初期化
	debugWindow.emplace(bulletPool, enemyPool);
}

std::unique_ptr<GameState> PlayState::update(float dt, sf::RenderWindow& window) {


	//解放対象を一時的にまとめる
	//弾
	std::vector<Bullet*> toFreeBullets;
	//敵
	std::vector<Enemy*> toFreeEnemies;

	//敵のスポーン管理
	static float spawnTimer = 0.f;
	spawnTimer += dt;

	if (spawnTimer >= 2.0f) {   //2秒ごとにスポーン
		spawnTimer = 0.f;
		Enemy* e = enemyPool.alloc();
		if (e) {
			//ランダムなX座標でスポーン
			float startX = static_cast<float>(rand() % 760);
			e->init(startX, -40.f); //画面上端からスポーン
		}
	}



	//======================
	// 衝突判定
	//======================

	//プレイヤのと画面端の衝突判定
	auto playerPos = player->getPosition();

	//画面サイズ
	const float screenWidth = 800.f;
	const float screenHeight = 600.f;

	//プレイヤサイズ
	const float playerSize = 40.f;

	//clamp(プレイヤの位置を画面内に制限)
	float x = playerPos.x;
	float y = playerPos.y;

	if (x < 0.f) x = 0.f;
	if (x > screenWidth - playerSize) x = screenWidth - playerSize;

	if (y < 0.f) y = 0.f;
	if (y > screenHeight - playerSize) y = screenHeight - playerSize;

	//位置を適応
	player->setPosition(x, y);


	//弾と敵の衝突判定
	bulletPool.forEachActive([&](Bullet& b) {

		bool hit = false;   //衝突フラグ(重複防止)

		enemyPool.forEachActive([&](Enemy& e) {

			if (hit) return;   //すでに衝突している場合はスキップ

			//弾の矩形を取得
			sf::FloatRect bulletRect(
				sf::Vector2f(b.getPosition().x, b.getPosition().y),
				sf::Vector2f(5.f, 15.f)
			);
			//敵の矩形を取得
			sf::FloatRect enemyRect(
				sf::Vector2f(e.getPosition().x, e.getPosition().y),
				sf::Vector2f(40.f, 40.f)
			);

			//衝突判定
			if (bulletRect.findIntersection(enemyRect)) {
				if (std::find(toFreeBullets.begin(), toFreeBullets.end(), &b) == toFreeBullets.end())
					toFreeBullets.push_back(&b); //弾を解放対象に追加
				if (std::find(toFreeEnemies.begin(), toFreeEnemies.end(), &e) == toFreeEnemies.end())
					toFreeEnemies.push_back(&e);  //敵を解放対象に追加
				hit = true;
				explosionPool.play();
				score += 100;
			}
		});
	});


	//プレイヤと敵の衝突判定
	bool playerHit = false;   //衝突フラグ

	enemyPool.forEachActive([&](Enemy& e) {
		//プレイヤの矩形を取得
		sf::FloatRect playerRect(
			sf::Vector2f(player->getPosition().x, player->getPosition().y),
			sf::Vector2f(40.f, 40.f)
		);

		//敵の矩形を取得
		sf::FloatRect enemyRect(
			sf::Vector2f(e.getPosition().x, e.getPosition().y),
			sf::Vector2f(40.f, 40.f)
		);

		if (playerRect.findIntersection(enemyRect)) {
			std::cout << "Player Hit!" << std::endl;
			playerHit = true;
		}
	});


	// =================
	// 解放
	// =================

	//解放対象の弾を解放
	for (Bullet* b : toFreeBullets) {
		bulletPool.free(b);
	}

	//解放対象の敵を解放
	for (Enemy* e : toFreeEnemies) {
		enemyPool.free(e);
	}

	//=============
	//イベント処理
	//=============
	while (const std::optional event = window.pollEvent())
	{
		//クローズイベント
		if (event->is<sf::Event::Closed>())
			window.close();

		//F1キーでデバックウィンドウの表示切り替え
		if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
			if (keyEvent->code == sf::Keyboard::Key::F1)
				debugWindow->toggleVisible();
		}
	}

	//====================
	// 更新
	//====================

	//プレイヤの更新
	player->update(dt);

	//弾の更新
	bulletPool.forEachActive([&](Bullet& b) {
		b.update(dt);
		//画面外チェック
		if (b.getPosition().y < 0) {
			//解放対象に追加
			toFreeBullets.push_back(&b);
		}
	});

	//敵の更新
	enemyPool.forEachActive([&](Enemy& e) {
		e.update(dt);
		//画面外チェック
		if (e.getPosition().y > 600.f) {
			//解放
			toFreeEnemies.push_back(&e);
		}
	});

	//デバックウィンドウの更新
	debugWindow->update();

	//画面のクリア
	window.clear(sf::Color::Black);

	//ゲームオーバー判定
	if (playerHit) {
		return std::make_unique<GameOverState>(score);
	}

	return nullptr;
}


void PlayState::draw(sf::RenderWindow& window) {
	player->draw(window);

	//弾の描画
	bulletPool.forEachActive([&](Bullet& b) {
		b.draw(window);
	});

	//敵の描画
	enemyPool.forEachActive([&](Enemy& e) {
		e.draw(window);
	});

	//スコアUIの描画
	scoreText->setString("Score: " + std::to_string(score));
	window.draw(*scoreText);

	//デバックウィンドウの描画
	debugWindow->render();
}