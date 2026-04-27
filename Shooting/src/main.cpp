#include "../include/Player.h"
#include "../include/Bullet.h"
#include "../include/Enemy.h"
#include "../include/ObjectPool.h"
#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
	//randomの初期化
	srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Shooter");
    
	static ObjectPool<Bullet, 128> bulletPool; //弾のオブジェクトプール
	static ObjectPool<Enemy, 128> enemyPool;   //敵のオブジェクトプール
    Player player(bulletPool);

	sf::Clock clock; //ゲームループのデルタタイム計測用

    //メインループ
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        //解放対象(画面外に出た弾)を一時的にまとめるベクター
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

        //====================
        // 解放
        //====================

        //解放対象の弾を解放
        for (Bullet* b : toFreeBullets) {
            bulletPool.free(b);
        }

		//解放対象の敵を解放
        for (Enemy* e : toFreeEnemies) {
            enemyPool.free(e);
		}

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

                ////衝突判定
                //if (bulletRect.position.x > enemyRect.position.x + enemyRect.size.x ||
                //    bulletRect.position.x + bulletRect.size.x < enemyRect.position.x ||
                //    bulletRect.position.y > enemyRect.position.y + enemyRect.size.y ||
                //    bulletRect.position.y + bulletRect.size.y < enemyRect.position.y) {
                //    //衝突なし
                //}
                //else {
                //    //衝突あり
                //    toFreeBullets.push_back(&b); //弾を解放対象に追加
                //    toFreeEnemies.push_back(&e);  //敵を解放対象に追加
                //    hit = true;
                //}
                
                //衝突判定(簡潔版)
                if (bulletRect.findIntersection(enemyRect)) {
                    toFreeBullets.push_back(&b); //弾を解放対象に追加
                    toFreeEnemies.push_back(&e);  //敵を解放対象に追加
                    hit = true;

                    //解放対象の弾を解放
                    for (Bullet* b : toFreeBullets) {
                        bulletPool.free(b);
                    }

                    //解放対象の敵を解放
                    for (Enemy* e : toFreeEnemies) {
                        enemyPool.free(e);
                    }
                }
            });
        });

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

        window.display();
    }

    return 0;
}