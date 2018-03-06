#define _USE_MATH_DEFINES

#include "Ship.h"
#include <math.h>
#include <iostream>


Ship::Ship(Audio *_a_obj) :
lives(numberOfLives),
timeSinceRespawn(0.f),
score(0),
Up(false),
Down(false),
Left(false), 
Right(false),
Fire(false),
timeSinceLastShot(0),
a_obj(_a_obj) {
	shape.setPointCount(3);
	shape.setPoint(0, sf::Vector2f(0, 0));
	shape.setPoint(1, sf::Vector2f(-5.f, 20.f));
	shape.setPoint(2, sf::Vector2f(5.f, 20.f));
	shape.setOrigin(0, -20 * 2/3);
	shape.setPosition(400, 290);
	shape.setRotation(90);
	shape.setFillColor(sf::Color::Black);
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness(1);
	ashape = &shape;
	speed.x = 0;
	speed.y = 0;

	m_Texture.loadFromFile("graphics/ship1.png");
	m_TexturePale.loadFromFile("graphics/shipPale.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(25,25);
	m_Sprite.setRotation(90);
	m_Sprite.setPosition(400, 290);
}

Ship::~Ship() {
}

void Ship::setWorldSize(sf::Vector2f size) {
	m_WorldSize = size;
}

void Ship::setPosition (sf::Vector2f position) {
	shape.setPosition(position);
	m_Sprite.setPosition(position);
}

void Ship::setFlags(bool up, bool down, bool left, bool right, bool fire){
	Up = up;
	Down = down;
	Left = left;
	Right = right;
	Fire = fire;
}

void Ship::setFlags(flags flag) {
	switch(flag) {
		case up :
			Up = true;
			break;

		case down :
			Down = true;
			break;

		case left :
			Left = true;
			break;

		case right :
			Right = true;
			break;

		case fire :
			Fire = true;
			timeSinceLastShot = 0;
			break;
	}
}

void Ship::clrFlags(flags flag) {
	switch(flag) {
		case up :
			Up = false;
			break;

		case down :
			Down = false;
			break;

		case left :
			Left = false;
			break;

		case right :
			Right = false;
			break;

		case fire :
			Fire = false;
			break;
	}
}

void Ship::update(sf::Time dt) {
	if (isRespawning()) {
		shape.setOutlineThickness(0.2f);
		m_Sprite.setTexture(m_TexturePale);
	}
	else {
		shape.setOutlineThickness(1.f);
		m_Sprite.setTexture(m_Texture);
	}

	speed.x = 200 * Right - 200 * Left;
	speed.y = 200 * Down - 200 * Up;

	if (Fire && (clk.getElapsedTime().asSeconds() - timeSinceLastShot > 0.2)) {
			bullets.push_back(std::make_shared<Bullet>(getPosition(), getDirection()));	
			a_obj -> soundFire();
			clk.restart();
			timeSinceLastShot = clk.getElapsedTime().asSeconds();	
	}

	sf::Vector2f pos = shape.getPosition();
	direction = static_cast<float>(shape.getRotation() * M_PI / 180 - M_PI);
	shape.move(speed * dt.asSeconds());
	if (pos.x < 0) {
		shape.setPosition(0, pos.y);
	}
	else if (pos.x > m_WorldSize.x) {
		shape.setPosition(m_WorldSize.x, pos.y);
	}
	else if (pos.y < 0) {
		reset();
		decrementLives();
	
		if (lives <= 0)
			return;
	}
	else if (pos.y > m_WorldSize.y) {
		reset();
		decrementLives();

		if(lives <= 0)
			return;
	}

	m_Sprite.setPosition(shape.getPosition());
	timeSinceRespawn += dt.asSeconds();
}

void Ship::reset() {
	shape.setPosition(m_WorldSize.x / 2.f, m_WorldSize.y / 2.f);
	m_Sprite.setPosition(m_WorldSize.x / 2.f, m_WorldSize.y / 2.f);
	shape.setRotation(90);
	speed.x = 0;
	speed.y = 0;
	timeSinceRespawn = 0.f;
}

bool Ship::isRespawning() {
	if (timeSinceRespawn < respawnTime) return true;
	else return false;
}

void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_Sprite, states);
}