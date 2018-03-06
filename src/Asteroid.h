#ifndef _ASTEROID_H
#define _ASTEROID_H

#define _USE_MATH_DEFINES
#include "Entity.h"

class Asteroid : public Entity {
public :
	enum Size {BIG, MEDIUM, SMALL};
	
	Asteroid(sf::Texture &m_Texture);
	Asteroid(Size _size, sf::Vector2f _speed, sf::Vector2f _position, sf::Texture &m_Texture);
	~Asteroid();

	Size getSize() const { return size; }
	sf::Vector2f getPosition() const { return shape->getPosition(); }
	sf::Vector2f getSpeed() const { return speed; }
	
	virtual void update(sf::Time dt) override;
	bool isAlive() { return alive;}

private :
	bool alive;
	sf::ConvexShape *shape;
	Size size;
	static const int numberOfSides = 10;
	static const int initialVelocity = 150;
	static std::map<Size, int> radiuses;
	void generateShape();

	void setWorldSize(sf::Vector2f size);
	sf::Vector2f m_WorldSize;
};

#endif