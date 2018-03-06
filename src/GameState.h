#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <SFML/Graphics.hpp>
#include "Game.h"

class Game;

class GameState {
public :

	GameState() {}
	GameState(Game* _game);
	virtual ~GameState();

	virtual void handleEvents() = 0;
	virtual void update(sf::Time dt) = 0;
	virtual void render() = 0;

protected :

	Game* game;
	sf::RenderWindow* window;
	sf::Vector2u resolution;
	sf::Font font;

	void centerText(sf::Text& txt, float height);
	sf::Text getText(unsigned size, float height, std::string str = "", 
	sf::Text::Style style = sf::Text::Style::Bold, bool centered = true);

private :

	static const std::string fontPath;
};

#endif