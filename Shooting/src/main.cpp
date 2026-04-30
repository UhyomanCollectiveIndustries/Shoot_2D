#include "../include/Player.h"
#include "../include/Bullet.h"
#include "../include/Enemy.h"
#include "../include/ObjectPool.h"
#include "../include/SoundPool.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SFML/Audio.hpp>

int main()
{
    //==========
    //サウンド
    //==========

    //BGM
    sf::Music bgm;
    if (!bgm.openFromFile("assets/sounds/maou_bgm_acoustic52.ogg")) {
        return -1;
    }
    bgm.setLooping(true);
    bgm.setVolume(50.f);
    bgm.play();

    //SE
    //SoundBufferの読み込み
    sf::SoundBuffer shootBuf, explosionBuf;

    //発射音
    if (!shootBuf.loadFromFile("assets/sounds/maou_bgm_acoustic52.ogg")) {
        return -1;
    }

    //爆発音
    if (!explosionBuf.loadFromFile("assets/sounds/maou_bgm_acoustic52.ogg")) {
        return -1;
    }

    //SoundPoolの作成
    SoundPool<8> shootPool;
    SoundPool<8> explosionPool;
    shootPool.setBuffer(shootBuf);
    explosionPool.setBuffer(explosionBuf);

	//randomの初期化
	srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Shoot_2D");
    
	static ObjectPool<Bullet, 128> bulletPool; //弾のオブジェクトプール
	static ObjectPool<Enemy, 128> enemyPool;   //敵のオブジェクトプール
    Player player(bulletPool,shootPool);

	sf::Clock clock; //ゲームループのデルタタイム計測用

    //スコア
    int score = 0;

    //========
    //UI設定
    //========

    //フォント
    sf::Font font;

    if (!font.openFromFile("assets/fonts/Orbitron-Black.ttf")) {
        //フォントファイルの読み込み失敗時
        return -1;
    }
    
    //スコアテキスト
    sf::Text scoreText(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({ 10.0f,10.f });

    //メインループ
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        //解放対象を一時的にまとめる
        //弾
        std::vector<Bullet*> toFreeBullets;
        //敵
        std::vector<Enemy*> toFreeEnemies;

        //敵のスポーン管理
        static float spawnTimer = 0.f;
        spawnTimer += deltaTime;

        if (spawnTimer >= 2.0f) {   //2秒ごとにスポーン
            spawnTimer = 0.f;
            Enemy* e = enemyPool.alloc();
            if (e) {
                //ランダムなX座標でスポーン
                float startX = static_cast<float>(rand() % 760);
                e->init(startX, -40.f); //画面上端からスポーン
            }
        }

        //====================
        // 更新
        //====================

        //プレイヤの更新
		player.update(deltaTime);

        //弾の更新
        bulletPool.forEachActive([&](Bullet& b) {
            b.update(deltaTime);
            //画面外チェック
            if (b.getPosition().y < 0) {
                //解放対象に追加
                toFreeBullets.push_back(&b);
            }
        });
        
        //敵の更新
        enemyPool.forEachActive([&](Enemy& e) {
            e.update(deltaTime);
			//画面外チェック
            if (e.getPosition().y > 600.f) {
                //解放
				toFreeEnemies.push_back(&e);
            }
        });

        //======================
        // 衝突判定
        //======================

		//弾と敵の衝突判定(AABB)
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
                    if(std::find(toFreeBullets.begin(),toFreeBullets.end(),&b) == toFreeBullets.end())
                        toFreeBullets.push_back(&b); //弾を解放対象に追加
                    if (std::find(toFreeEnemies.begin(), toFreeEnemies.end(), &e) == toFreeEnemies.end())
                        toFreeEnemies.push_back(&e);  //敵を解放対象に追加
                    hit = true;
                    explosionPool.play();
                    score += 100;
                }
            });
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

		//クローズイベントの処理
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

		//画面のクリア
        window.clear(sf::Color::Black);

        //=================
        // 描画
        //=================

		//プレイヤの描画
		player.draw(window);

        //弾の描画
        bulletPool.forEachActive([&](Bullet& b) {
            b.draw(window);
		});

        //敵の描画
        enemyPool.forEachActive([&](Enemy& e) {
            e.draw(window);
        });

        //スコアUIの描画
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        window.display();
    }

    return 0;
}