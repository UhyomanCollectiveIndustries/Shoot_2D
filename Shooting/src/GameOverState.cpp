#include "../include/GameOverState.h"
#include "../include/TitleState.h"
#include <iostream>

GameOverState::GameOverState(int resultScore): resultScore(resultScore) {
	//フォントロード
    if (!font.openFromFile("assets/fonts/Orbitron-Black.ttf")) {
        std::cerr << "Font load failed" << std::endl;
    }

	//ゲームオーバテキスト
	gameOverText.emplace(font);
	gameOverText->setCharacterSize(48);
	gameOverText->setFillColor(sf::Color::White);
	gameOverText->setString("Game Over\nScore:" + std::to_string(resultScore));
	//テキストサイズを取得してから、位置を計算して中央に配置
	auto bounds = gameOverText->getLocalBounds();
	gameOverText->setOrigin(sf::Vector2f(
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	));
	gameOverText->setPosition(
		sf::Vector2f(400.f, 200.f));

	//プロンプトテキスト
	promptText.emplace(font);
	promptText->setCharacterSize(24);
	promptText->setFillColor(sf::Color::White);
	promptText->setString("Press ENTER to Go Title");
	promptText->setPosition(sf::Vector2f(400.f, 300.f));
}


std::unique_ptr<GameState> GameOverState::update(float dt, sf::RenderWindow& window)
{
    //エンターキーが押されたタイトル画面に遷移
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        return std::make_unique<TitleState>();
    }
    return nullptr;
}

void GameOverState::draw(sf::RenderWindow& window)
{
    window.draw(*gameOverText);
	window.draw(*promptText);
}
