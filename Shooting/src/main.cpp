#include "../include/Player.h"
#include "../include/Bullet.h"
#include "../include/ObjectPool.h"
#include <SFML/Graphics.hpp>
#include <vector>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Shooter");
    
	ObjectPool<Bullet, 256> bulletPool; //弾のオブジェクトプール
    Player player(bulletPool);

	sf::Clock clock; //ゲームループのデルタタイム計測用

    //メインループ
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        //解放対象(画面外に出た弾)を一時的にまとめるベクター
        std::vector<Bullet*> toFree;

        //プレイヤの更新
		player.update(deltaTime);

        //弾の更新
        bulletPool.forEachActive([&](Bullet& b) {
            b.update(deltaTime);
            //画面外チェック
            if (b.getPosition().y < 0) {
                //解放対象に追加
                toFree.push_back(&b);
            }
        });

        //解放対象の弾を解放
        for (Bullet* b : toFree) {
            bulletPool.free(b);
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

        window.display();
    }

    return 0;
}