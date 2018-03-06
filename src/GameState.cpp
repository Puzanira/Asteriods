#include "GameState.h"
#include <iostream>

const std::string GameState::fontPath = "./fonts/Hyperspace Bold.ttf";

GameState::GameState(Game* _game) :
game(_game),
window(_game->getWindow()) {
	game = _game;
	this->window = _game->getWindow();
	resolution = window->getSize();

	if (!font.loadFromFile(fontPath)) {
		std::cerr << " Error loading font\n";
	}
}

GameState::~GameState() {
}

void GameState::centerText(sf::Text& txt, float height) {
	txt.setPosition(resolution.x/2 - txt.getGlobalBounds().width / 2, height);
}

sf::Text GameState::getText(unsigned size, float height, std::string str, sf::Text::Style style, bool centered) {
	sf::Text txt;
	txt.setFont(font);
	txt.setCharacterSize(size);
	txt.setString(str);

	if (centered) {
		centerText(txt, height);
	}
	else {
		txt.setPosition(0.f, height);
	}
	txt.setStyle(style);

	return txt;
}