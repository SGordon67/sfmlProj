#include "Glyph.h"
#include "Station.h"

Glyph::Glyph(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
	     float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
	     int hp, int maxHP, bool used)
	: Station(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP, used)
{
}
