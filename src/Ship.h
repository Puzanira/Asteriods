#ifndef _SHIP_H
#define _SHIP_H

#include "Entity.h"
#include "Bullet.h"
#include "Audio.h"
#include <memory>

enum flags{up,down,left,right,fire};

class Ship : public Entity {
public :

	Ship(Audio *_a_obj);
	~Ship();

	void reset();
	void decrementLives() { lives--; }

	int getLives() const { return lives; }
	sf::Vector2f getPosition() const { return shape.getPosition(); }
	float getDirection() const { return direction; }
	unsigned int getScore() { return score; }
	bool engineOn(){ return ((speed.x != 0) || (speed.y != 0)); }
	bool isRespawning();
	void updateScore(int sc) { score += sc; }
	virtual void update(sf::Time dt) override;

	void setFlags(flags flag);
	void setFlags(bool up, bool down, bool left, bool right, bool fire);
	void clrFlags(flags flag);
	std::vector<std::shared_ptr<Bullet>> bullets;
	float timeSinceLastShot;
	bool isFire(){return Fire;}

	void setPosition(sf::Vector2f position);
	void setWorldSize(sf::Vector2f size);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private :

	Audio *a_obj;
	sf::Clock clk;
	sf::ConvexShape shape;
	static const int numberOfLives = 3;
	const float respawnTime = 2.f;
	float timeSinceRespawn;
	float direction;
	int lives;
	unsigned score;
	bool Up;
	bool Down;
	bool Left;
	bool Right;
	bool Fire;

	sf::Vector2f m_WorldSize;

	sf::Texture m_Texture;
	sf::Texture m_TexturePale;
	sf::Sprite  m_Sprite;
};

#endif