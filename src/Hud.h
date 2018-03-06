#ifndef _HUD_H
#define _HUD_H

#include <SFML/Graphics.hpp>
#include "Ship.h"

class Hud : public sf::Drawable {
public :

	Hud(Ship * _ship, sf::Font& _font);
	~Hud();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

private :

	sf::Font font;
	sf::Text text;
	Ship *ship;
};

#endif

