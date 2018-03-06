#ifndef _GAMEPLAY_STATE_H
#define _GAMEPLAY_STATE_H


#include "World.h"

class GameplayState : public GameState {
public :

	GameplayState(Game* _game);
	virtual ~GameplayState();

	virtual void update(sf::Time dt) override;
	virtual void render() override;
	virtual void handleEvents() override;

private :

	World m_World;
	bool gameRunning;
	sf::Text soundSwitchButton;
	sf::Text musicSwitchButton;
	sf::Text nextSongButton;
};

#endif

