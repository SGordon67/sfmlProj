#include <iostream>

#include "Player.h"
#include "SFML/System/Vector2.hpp"
#include "enums.h"
#include "globals.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

Player::Player()
    : Entity(sf::Vector2f(900, 500), d_playerSize, M_PI/2, RenderLayer::Main, &playerTexture,
            d_playerMass, d_playerRadius, d_playerVelocity, d_playerAcceleration, d_playerAngularVelocity, d_playerMaxVelocity, d_playerDrag,
            d_playerHP, d_playerMaxHP)
      , m_angularAcceleration(d_playerAngularAcceleration)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 1.6f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	// std::cout << "Player constructed" << std::endl;
}
Player::Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
        int hp, int maxHP, float angularAccleration)
	: Entity(position, size, rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag,
            hp, maxHP)
	, m_angularAcceleration(angularAccleration)
{
	m_sprite.setOrigin({getSize().x / 2.f, getSize().y / 1.6f}); // roughtly the point of rotation I want with the current sprite in relation to the collision
	// std::cout << "Player constructed" << std::endl;
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

void Player::updateWeapons(float deltaTime, QuadTree& quadTree)
{
    for(auto& weapon : m_weapons)
    {
        if(weapon)
        {
            weapon->update(deltaTime, *this, quadTree);
        }
    }
}

void Player::addWeapon(std::unique_ptr<Weapon> weapon, int slot)
{
    if(slot >= 0 && slot < 4)
    {
        m_weapons[slot] = std::move(weapon);
    }
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
void Player::update()
{
    // playerUpdate();
}
