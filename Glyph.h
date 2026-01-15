#ifndef GLYPH_H
#define GLYPH_H

#include "Station.h"

class Glyph : public Station
{
protected:
public:
	Glyph(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
		float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
		int hp, int maxHP, bool used);
};

#endif

