#include "Hud.h"
#include <iostream>

Hud::Hud(Ship *_ship, sf::Font& _font) : 
ship(_ship),
font(_font) {
	text.setFont(font);
	text.setPosition(0.f, 0.f);
	text.setColor(sf::Color::White);
	text.setCharacterSize(20);
}

Hud::~Hud() {
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(text, states);
}

void Hud::update() {
	text.setString("Score: " + std::to_string(ship->getScore()) + 
		"\nLives: " + std::to_string(ship->getLives()));
}