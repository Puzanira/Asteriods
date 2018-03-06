#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "Game.h"
#include "GameplayState.h"
#include "HighscoresState.h"
#include "GameOverState.h"
#include "MainMenuState.h"

const sf::Time Game::timePerFrame = sf::seconds(1.f / 60.f);

Game::Game(controller *ptr) :
contr(ptr) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(sf::VideoMode(800, 600), "Asteroids", sf::Style::Default, settings);

	currentState = new MainMenuState(this);
}

Game::~Game() {
	delete currentState;
}

void Game::run() {
	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;
	
	while (window.isOpen()) {
		a_obj.playMusic();
		currentState->handleEvents();
		elapsedTime += clock.restart();

		while (elapsedTime > timePerFrame) {
			elapsedTime -= timePerFrame;
			currentState->handleEvents();
			currentState->update(timePerFrame);
		}

		currentState->render();
	}

	contr -> switch_off();
}

void Game::changeState(States nextState, unsigned score) {
	delete currentState;

	switch (nextState) {
		case States::GAMEPLAY :
			currentState = new GameplayState(this);
			break;
		case States::HIGHSCORES :
			currentState = new HighscoresState(this);
			break;
		case States::MAIN_MENU:
			currentState = new MainMenuState(this);
			break;
		case States::GAME_OVER:
			currentState = new GameOverState(this, score);
			break;
	}
}