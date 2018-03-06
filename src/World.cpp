#include "World.h"

#include <iostream>
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

World::World(sf::RenderWindow& window, sf::Font& font, Game *_game) :
m_Window(window),
m_WorldView(window.getDefaultView()),
m_WorldBounds(0.f, 0.f, m_WorldView.getSize().x, m_WorldView.getSize().y),
m_SpawnPosition(m_WorldView.getSize().x / 2.f, m_WorldView.getSize().y / 2.f),
m_BGBounds(0.f, 0.f, 3000.f, 2000.f),
currentWave(0),
timeSinceLastShot(0),
hud(&ship, font),
game(_game),
ship(&(_game -> a_obj)) {
	buildScene();

	m_WorldView.setCenter(m_SpawnPosition);
	m_Texture_asteroid.loadFromFile("graphics/meteor.png");
}

void World::buildScene() {
	m_BackgroundTexture.loadFromFile("graphics/background.png");
	sf::IntRect         iBounds(m_BGBounds);
	m_BackgroundTexture.setRepeated(true);
	m_BackgroundSprite = sf::Sprite(m_BackgroundTexture, iBounds);

	viewStart = sf::Vector2f(m_BGBounds.left + (m_BGBounds.width / 2), m_BGBounds.top + (m_BGBounds.height / 2));
    m_BGView.setCenter(viewStart);
    spriteStart = sf::Vector2f(m_BGBounds.left, m_BGBounds.top);

	ship.setPosition(m_SpawnPosition);
	ship.setWorldSize(sf::Vector2f(m_WorldView.getSize().x, m_WorldView.getSize().y));
	createNewWave();
}

void World::createNewWave() {
	for (int i = 0; i < floor(currentWave * 0.7 * 6); i++) {
		asteroids.push_back(std::make_shared<Asteroid>(m_Texture_asteroid));
	}
	currentWave++;
}

void World::handleCollisions() {
	for (unsigned i = 0; i < asteroids.size(); i++) {
		if (!ship.isRespawning() && asteroids.at(i)->intersect(ship)) {
			ship.reset();
			ship.decrementLives();

			if (ship.getLives() <= 0) {
				game->changeState(Game::States::GAME_OVER, ship.getScore());

				return;
			}
		}
		for (unsigned j = 0; j < ship.bullets.size(); j++) {
			if (!(ship.bullets.at(j)->isAlive())) {
				ship.bullets.erase(ship.bullets.begin() + j);
			}
			else if (asteroids.size() > i && asteroids.at(i)->intersect(*(ship.bullets.at(j)))) {
				switch (asteroids.at(i)->getSize()) {
				case Asteroid::Size::SMALL :
					game -> a_obj.soundExplosion(Asteroid::Size::SMALL);
					ship.updateScore(100);
					break;

				case Asteroid::Size::MEDIUM :
					game -> a_obj.soundExplosion(Asteroid::Size::MEDIUM);
					ship.updateScore(40);
					break;

				case Asteroid::Size::BIG :
					game -> a_obj.soundExplosion(Asteroid::Size::BIG);
					ship.updateScore(20);
					break;
				}

				if (asteroids.at(i)->getSize() != Asteroid::Size::SMALL) {
					sf::Vector2f speed = asteroids.at(i)->getSpeed();
					sf::Vector2f position = asteroids.at(i)->getPosition();
					asteroids.push_back(std::make_shared<Asteroid>(static_cast<Asteroid::Size>(asteroids.at(i)->getSize() + 1),
						sf::Vector2f{ speed.x * 2, -speed.y * 2 }, position, m_Texture_asteroid));
					asteroids.push_back(std::make_shared<Asteroid>(static_cast<Asteroid::Size>(asteroids.at(i)->getSize() + 1),
						sf::Vector2f{ -speed.x * 2, speed.y * 2 }, position, m_Texture_asteroid));
				}

				asteroids.erase(asteroids.begin() + i);
				ship.bullets.erase(ship.bullets.begin() + j);
			}
		}
	}
}

void World::update(sf::Time dt) {
	m_BGView.move(0.2f, 0.0f); 
	const sf::Vector2f viewOffset(viewStart - m_BGView.getCenter());
    sf::Vector2f spriteOffset;
    spriteOffset.x = floor(viewOffset.x / m_BackgroundTexture.getSize().x) * m_BackgroundTexture.getSize().x;
    spriteOffset.y = floor(viewOffset.y / m_BackgroundTexture.getSize().y) * m_BackgroundTexture.getSize().y;
    m_BackgroundSprite.setPosition(spriteStart - spriteOffset);
    	
    ship.setFlags(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || game -> contr -> get_st_up(), 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || game -> contr -> get_st_down(), 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || game -> contr -> get_st_left(), 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || game -> contr -> get_st_right(), 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || game -> contr -> get_st_fire());

	for (unsigned i = 0; i < asteroids.size(); i++) {
		if(!asteroids[i]->isAlive()) {
			asteroids.erase(asteroids.begin() + i);
		}
	}

	if (asteroids.empty()) {
		createNewWave();
	}

	for (auto a : asteroids) {
		a->update(dt);
	}

	for (auto b : ship.bullets) {
		b->update(dt);
	}

	ship.update(dt);
	hud.update();
	handleCollisions();
}

void World::draw(){
	m_Window.setView(m_BGView);
	m_Window.draw(m_BackgroundSprite);

	m_Window.setView(m_WorldView);

	for (auto a : asteroids) {
		m_Window.draw(*a);
	}

	for (auto b : ship.bullets) {
		m_Window.draw(*b);
	}

	m_Window.draw(ship);
	m_Window.draw(hud);
}


int World::getLives(){
	return ship.getLives();
}

int World::getScore(){
	return ship.getScore();
}