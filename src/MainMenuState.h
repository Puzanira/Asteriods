#ifndef _MAIN_MENU_STATE_H
#define _MAIN_MENU_STATE_H

#include <memory>
#include "GameState.h"

class MainMenuState : public GameState {
public :

	MainMenuState(Game *_game);
	virtual ~MainMenuState();

	virtual void handleEvents() override;
	virtual void update(sf::Time dt) override;
	virtual void render() override;

private :

	sf::Text title;
	sf::Text playButton;
	sf::Text highscoreButton;
	sf::Text outButton;
};

#endif
