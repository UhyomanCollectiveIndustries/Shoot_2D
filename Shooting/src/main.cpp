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

        //解放対象の弾を解放
        for (Bullet* b : toFreeBullets) {
            bulletPool.free(b);
        }

        //敵の更新
        enemyPool.forEachActive([&](Enemy& e) {
            e.update(deltaTime);
			//画面外チェック
            if (e.getPosition().y > 600.f) {
                //解放
				toFreeEnemies.push_back(&e);
            }
        });

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