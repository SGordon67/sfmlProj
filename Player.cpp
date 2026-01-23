#include <iostream>

#include "Player.h"
#include "SFML/System/Vector2.hpp"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

Player::Player()
    : Entity(sf::Vector2f(900, 500), sf::Vector2i(24, 30), M_PI/2, RenderLayer::Main, "art/basicSpriteL.png", 10, 24/2.f, 
            sf::Vector2f(0, 0), 5000, 0, 500, 100, 100)
{
    m_angularAcceleration = degreesToRadians(4);

	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 1.6f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	std::cout << "Player constructed" << std::endl;
}
Player::Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
        int hp, int maxHP, float angularAccleration)
	: Entity(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
	, m_angularAcceleration(angularAccleration)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 1.6f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	std::cout << "Player constructed" << std::endl;
}

float Player::getAngularAcceleration()
{
	return m_angularAcceleration;
}
void Player::setAngularAcceleration(float angularAcceleration)
{
	m_angularAcceleration = angularAcceleration;
}

void Player::printInfo()
{
	std::cout << std::endl << "Player Info: " << std::endl;
    std::cout << "Health: (" << getHP() << ")" << std::endl;
	// std::cout << "Position: (" << getPosition().x << ", " << getPosition().y << ")" << std::endl;
	// std::cout << "Velocity: (" << getVelocity().x << ", " << getVelocity().y << ")" << std::endl;
}

void Player::updateRotation()
{
	rotate(getAngularVelocity());
}

void Player::playerUpdate(bool* (&buttons)[numButtons])
{
	// handle rotation
	float potentialRotation = 0;
	if(*buttons[3]) // left
	{
		potentialRotation -= getAngularAcceleration();
	}
	if(*buttons[4]) // right
	{
		potentialRotation += getAngularAcceleration();
	}
	if(potentialRotation != 0) 
	{
		setAngularVelocity(potentialRotation);
		updateRotation();
	}

	// handle acceleration
	int accel = 0;
	bool backward = false;
	if(*buttons[1] && !*buttons[2])
	{
		accel = getAcceleration();
	} else if(!*buttons[1] && *buttons[2])
	{
		backward = true;
		accel = -getAcceleration();
	}
	updateVelocity(accel, backward);
	updatePosition(FixedDeltaTime);

	// this->printInfo();
}
void Player::physicalUpdate()
{
}
