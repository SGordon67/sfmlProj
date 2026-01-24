#include "Ball.h"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

Ball::Ball()
    : PhysicalObject(sf::Vector2f(0.1, 0.1), sf::Vector2i(17, 17), M_PI/2, RenderLayer::Main, &meteorTexture, 
            10, 8, sf::Vector2f(0, 0), 0, 0, 500)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 2.f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	// std::cout << "Player constructed" << std::endl;
}
Ball::Ball(sf::Vector2f position)
    : PhysicalObject(position, sf::Vector2i(17, 17), M_PI/2, RenderLayer::Main, &meteorTexture, 
            10, 8, sf::Vector2f(0, 0), 0, 0, 500)
{
}
Ball::Ball(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
	   float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity)
	: PhysicalObject(position, size, rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, angularVelocity, maxVelocity)
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
