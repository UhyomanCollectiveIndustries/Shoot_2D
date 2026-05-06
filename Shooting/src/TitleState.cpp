#include "../include/TitleState.h"
#include "../include/PlayState.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>


TitleState::TitleState() {

	//フォントロード
	if (!font.openFromFile("assets/fonts/Orbitron-Black.ttf")) {
		std::cerr << "Font load failed" << std::endl;
	}

    //タイトルテキスト
	titleText.emplace(font);
    titleText->setCharacterSize(48);
	titleText->setFillColor(sf::Color::White);
	titleText->setString("SHOOTING GAME");
	//テキストサイズを取得してから、位置を計算して中央に配置
	auto bounds = titleText->getLocalBounds();
	titleText->setOrigin(sf::Vector2f(
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	));
	titleText->setPosition(
		sf::Vector2f(400.f,200.f));

    //プロンプトテキスト
	promptText.emplace(font);
	promptText->setCharacterSize(24);
	promptText->setFillColor(sf::Color::White);
	promptText->setString("Press SPACE to Start");
	promptText->setPosition(sf::Vector2f(400.f, 300.f));
}

std::unique_ptr<GameState> TitleState::update(float dt, sf::RenderWindow& window) {

	//スペースキーが押されたらゲーム画面に遷移
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        return std::make_unique<PlayState>();
    }
	return nullptr;
}

void TitleState::draw(sf::RenderWindow& window) {
	window.draw(*titleText);
	window.draw(*promptText);
}