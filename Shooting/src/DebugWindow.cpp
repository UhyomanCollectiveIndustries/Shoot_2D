#include "../include/DebugWindow.h"
#include <string>

DebugWindow::DebugWindow(
	ObjectPool<Bullet, 128>& bPool,ObjectPool<Enemy, 128>& ePool) 
	:	bPool(bPool),ePool(ePool),visible(false) {
	//ウィンドウ設定
	window.create(sf::VideoMode({ 800, 600 }), "Debug Window");
	window.setVisible(false);

	//フォントのロード
	if (!font.openFromFile("assets/fonts/Orbitron-Black.ttf")) {
		//エラー
	}

	//テキストの初期設定
	bulletText.emplace(font);
	bulletText->setCharacterSize(18);
	bulletText->setFillColor(sf::Color::Black);
	bulletText->setPosition(sf::Vector2f(10.f, 20.f));

	enemyText = bulletText;	//コピー
	enemyText->setPosition(sf::Vector2(10.f, 60.f));	//位置のみ調整
}

void DebugWindow::toggleVisible() {
	visible = !visible;
	window.setVisible(visible);
}

bool DebugWindow::isVisible() const {
	return visible;
}

void DebugWindow::update() {
	if (!visible) return;

	//デバックウィンドウのイベント処理
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			toggleVisible();
		}
	}

	//テキスト情報の更新
	bulletText->setString(
		"Bullet: " + std::to_string(bPool.getActiveCount()) +
		" / " + std::to_string(bPool.getCapacity()) +
		" (" + std::to_string((int)(bPool.getUsageRate() * 100)) + "%)"
	);
	enemyText->setString(
		"Enemy: " + std::to_string(ePool.getActiveCount()) +
		" / " + std::to_string(ePool.getCapacity()) +
		" (" + std::to_string((int)(ePool.getUsageRate() * 100)) + "%)"
	);
}

void DebugWindow::render() {
	if (!visible) return;

	//背景
	window.clear(sf::Color(50, 50, 50));
	//使用率
	window.draw(*bulletText);
	window.draw(*enemyText);
	window.display();
}