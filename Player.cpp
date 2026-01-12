#include <iostream>

#include "Player.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

Player::Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, int hp, int maxHP, float angularAccleration)
	: Entity(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
	, m_angularAcceleration(angularAccleration)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 1.6f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	std::cout << "Player constructed" << std::endl;
}

void Player::printInfo()
{
	std::cout << std::endl << "Player Info: " << std::endl;
	std::cout << "Position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
	std::cout << "Velocity: (" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
}

void Player::updateRotation()
{
	rotate(m_angularVelocity);
}

void Player::playerUpdate(bool* (&buttons)[numButtons])
{
	// handle rotation
	float potentialRotation = 0;
	if(*buttons[3]) // left
	{
		potentialRotation -= m_angularAcceleration;
	}
	if(*buttons[4]) // right
	{
		potentialRotation += m_angularAcceleration;
	}
	if(potentialRotation != 0) 
	{
		m_angularVelocity = potentialRotation;
		updateRotation();
	}

	// handle acceleration
	int accel = 0;
	bool backward = false;
	if(*buttons[1] && !*buttons[2])
	{
		accel = m_acceleration;
	} else if(!*buttons[1] && *buttons[2])
	{
		backward = true;
		accel = -m_acceleration;
	}
	updateVelocity(accel, backward);
	updatePosition(FixedDeltaTime);

	// this->printInfo();
}
