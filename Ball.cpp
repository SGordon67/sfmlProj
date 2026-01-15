#include "Ball.h"
#include "SFML/System/Vector2.hpp"

Ball::Ball(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
	   float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity)
	: PhysicalObject(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, angularVelocity, maxVelocity)
{
}
Ball::Ball(const Ball& other) // copy constructor
	: PhysicalObject(other)
{
}
Ball::Ball(Ball&& other) noexcept // move constructor
	: PhysicalObject(std::move(other))
{
}
