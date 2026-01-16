#include <iostream>

#include "HealthCrate.h"
#include "BasicObject.h"
#include "Player.h"
#include "SFML/System/Vector2.hpp"

HealthCrate::HealthCrate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
		float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
		float interactRadius, bool used, int health)
    : Crate(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, interactRadius, used)
	, m_health(health)
{
}
HealthCrate::HealthCrate(const HealthCrate& other) // copy constructor
	: Crate(other)
	, m_health(other.m_health)
{
}
HealthCrate::HealthCrate(HealthCrate&& other) noexcept // move constructor
	: Crate(std::move(other))
	, m_health(other.m_health)
{
}

int HealthCrate::getValue()
{
    return m_health;
}
void HealthCrate::setValue(int value)
{
    m_health = value;
}

int HealthCrate::getObjectID() const
{
    return BasicObject::getObjectID();
}

sf::Vector2f HealthCrate::getPosition() const
{
	return BasicObject::getPosition();
}

bool HealthCrate::canInteract() const
{
	return !m_used;
}

float HealthCrate::getInteractionRadius() const
{
	return m_interactRadius;
}

void HealthCrate::interact(Player& player)
{
	m_used = true;
	std::cout << "Player ID (" << player.getObjectID() << ") interacting with object (" << m_objectID << ")" << std::endl;
}
