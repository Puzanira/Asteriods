#include <iostream>
#include "Bullet.h"
#include <cmath>


Bullet::Bullet(sf::Vector2f position, float direction) :
travelled(0),
mIsAlive(true) {
	shape.setRadius(1.f);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(position);
	speed.x = -sin(direction) * bulletSpeed;
	speed.y = cos(direction) * bulletSpeed;
	ashape = &shape;
}

Bullet::~Bullet() {
}

void Bullet::update(sf::Time dt) {
	shape.move(speed.x * dt.asSeconds(), speed.y * dt.asSeconds());
	travelled += bulletSpeed * dt.asSeconds();
	
	if (travelled > maxDistance) {
		mIsAlive = false;
	}
}