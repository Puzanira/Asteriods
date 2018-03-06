#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include "GameState.h"
#include "Ship.h"
#include "Hud.h"
#include "Bullet.h"
#include "Asteroid.h"

class World {
public :

	explicit World(sf::RenderWindow& m_Window, sf::Font& font, Game *_game);
	void update(sf::Time dt);
	void draw();

	int getLives();
	int getScore();

private :

	sf::RenderWindow& 		m_Window;
	sf::View 				m_WorldView;

	sf::FloatRect 			m_WorldBounds;
	sf::Vector2f 			m_SpawnPosition;

	void buildScene();

	void handleCollisions();
	void createNewWave();

	sf::View 				m_BGView; 
	sf::Sprite 				m_BackgroundSprite;
	sf::Texture 			m_BackgroundTexture;
	sf::FloatRect 			m_BGBounds;
	sf::Vector2f 			viewStart;
	sf::Vector2f 			spriteStart;

	int 					currentWave;
	float 					timeSinceLastShot;

	Ship ship;
	Hud hud;
	std::vector<std::shared_ptr<Asteroid>> asteroids;
	Game *game;
	sf::Texture m_Texture_asteroid;
};

#endif //WORLD_HPP