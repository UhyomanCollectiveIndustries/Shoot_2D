#pragma once
#include <SFML/Audio.hpp>
#include <array>
#include <optional>

template<int MAX>
class SoundPool {
public:
	//サウンドバッファをセット
	void setBuffer(const sf::SoundBuffer& buffer) {
		buf = &buffer;
		for (int i = 0; i < MAX; i++) {
			sounds[i].emplace(*buf);
		}
	}

	//空きサウンドを見つけて再生
	void play() {
		for (int i = 0; i < MAX; i++) {
			//停止中のサウンドを探す
			if (sounds[i]->getStatus() == sf::Sound::Status::Stopped) {
				sounds[i]->play();
				return;
			}
		}
	}

private:
	const sf::SoundBuffer* buf = nullptr;
	std::optional<sf::Sound> sounds[MAX];
};