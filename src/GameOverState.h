#ifndef _GAME_OVER_STATE_H
#define _GAME_OVER_STATE_H

#include "GameState.h"

class GameOverState : public GameState {
public :

	GameOverState(Game* _game, unsigned _score);
	virtual ~GameOverState();

	virtual void handleEvents() override;
	virtual void update(sf::Time dt) override {};
	virtual void render() override ;

private :

	std::string name;
	unsigned score;

	sf::Text title;
	sf::Text confirmButton;
	sf::Text cancelButton;
	sf::Text nameField;
	sf::Text enterNameText;

	void addScore();
};

#endif