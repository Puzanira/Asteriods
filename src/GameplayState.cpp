#include "GameplayState.h"
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Game.h"

GameplayState::GameplayState(Game* _game) :
GameState(_game),
gameRunning(true),
m_World(*window, font, _game) {
	soundSwitchButton.setPosition(600.f, 5.f);
	soundSwitchButton.setString("Sounds on");
	soundSwitchButton.setFont(font);
	soundSwitchButton.setScale(0.5f, 0.5f);
	musicSwitchButton.setPosition(700.f, 5.f);
	musicSwitchButton.setString("Music on");
	musicSwitchButton.setFont(font);
	musicSwitchButton.setScale(0.5f, 0.5f);
	nextSongButton.setPosition(500.f, 5.f);
	nextSongButton.setString("Next song");
	nextSongButton.setScale(0.5f, 0.5f);
	nextSongButton.setFont(font);
}

GameplayState::~GameplayState() {}

void GameplayState::handleEvents() {
	if (m_World.getLives() <= 0) {
		game->changeState(Game::States::GAME_OVER, m_World.getScore());

		return;
	}

	sf::Event event;

	while (window->pollEvent(event)) {
		if (event.type == sf::Event::EventType::Closed) {
			window->close();
		}

		if ((event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::EventType::KeyPressed)) {
			if (soundSwitchButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || event.key.code == sf::Keyboard::S) {
				if (game -> a_obj.getSoundVolume()){
					soundSwitchButton.setString("Sounds off");
					game -> a_obj.setSoundVolume(0);
				}
				else {
					soundSwitchButton.setString("Sounds on");
					game -> a_obj.setSoundVolume(100);
				}
			}
			else if (musicSwitchButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || event.key.code == sf::Keyboard::M) {
				if (game -> a_obj.getMusicVolume()) {
					musicSwitchButton.setString("Music off");
					game -> a_obj.setMusicVolume(0);
				}
				else {
					musicSwitchButton.setString("Music on");
					game -> a_obj.setMusicVolume(100);
				}
			}
			else if (nextSongButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) || event.key.code == sf::Keyboard::N) {
				game -> a_obj.stopSong();
				game -> a_obj.incIndex();
			}
			else if(event.key.code == sf::Keyboard::U) {
				game -> a_obj.incMusicVolume();
			}
			else if(event.key.code == sf::Keyboard::D) {
				game -> a_obj.decMusicVolume();
			}
			else if(event.key.code == sf::Keyboard::Escape) {
				window->close();
			}
		}
	}	
}

void GameplayState::update(sf::Time dt) {
	
	if (!(game -> a_obj.getMusicVolume())) {
		musicSwitchButton.setString("Music off");
	}
	else {
		musicSwitchButton.setString("Music on");
	}
	
	m_World.update(dt);
}

void GameplayState::render() {
	window->clear();

	m_World.draw();
	window -> draw(nextSongButton);
	window -> draw(soundSwitchButton);
	window -> draw(musicSwitchButton);

	window->display();
}