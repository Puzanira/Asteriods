#ifndef _BULLET_H
#define _BULLET_H

#include "Entity.h"

class Bullet : public Entity {
public:
	
	Bullet(sf::Vector2f position, float direction);
	~Bullet();

	bool isAlive() const { return mIsAlive; }
	virtual void update(sf::Time dt) override;

private:

	sf::CircleShape shape;

	const float bulletSpeed = 450.f;
	const float maxDistance = 450.f;
	
	float travelled;
	bool mIsAlive;
};

#endif
