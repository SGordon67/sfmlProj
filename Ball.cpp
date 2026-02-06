#include "Ball.h"
#include "PhysicalObject.h"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

Ball::Ball()
    : PhysicalObject(sf::Vector2f(0.1, 0.1), d_ballSize, M_PI/2, RenderLayer::Main, &meteorTexture,
            d_ballMass, d_ballRadius, d_ballVelocity, d_ballAcceleration, d_ballAngularVelocity, d_ballMaxVelocity, d_ballDrag)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 2.f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
}
Ball::Ball(sf::Vector2f position)
    : PhysicalObject(position, d_ballSize, M_PI/2, RenderLayer::Main, &meteorTexture,
            d_ballMass, d_ballRadius, d_ballVelocity, d_ballAcceleration, d_ballAngularVelocity, d_ballMaxVelocity, d_ballDrag)
{
}
Ball::Ball(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
	   float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity, float drag)
	: PhysicalObject(position, size, rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, angularVelocity, maxVelocity, drag)
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
