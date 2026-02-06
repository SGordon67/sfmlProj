#include "Glyph.h"
#include "Station.h"

Glyph::Glyph(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
	     float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
	     int hp, int maxHP, bool used)
	: Station(position, size, rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag, 
            hp, maxHP, used)
{
}
