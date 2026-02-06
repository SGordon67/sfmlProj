#include <iostream>
#include <memory>

#include "HealthCrate.h"
#include "BasicObject.h"
#include "Player.h"
#include "SFML/System/Vector2.hpp"
#include "enums.h"
#include "globals.h"


HealthCrate::HealthCrate()
    : Crate(sf::Vector2f(0, 0), d_crateSize, M_PI/2, RenderLayer::Main, &crateTexture,
            d_crateMass, d_crateRadius, d_crateVelocity, d_crateAcceleration, d_crateAngularVelocity, d_crateMaxVelocity, d_crateDrag,
            d_crateInteractRadius, false)
      , m_health(d_healthCrateHealth)
{
}
HealthCrate::HealthCrate(sf::Vector2f position)
    : Crate(position, d_crateSize, M_PI/2, RenderLayer::Main, &crateTexture,
            d_crateMass, d_crateRadius, d_crateVelocity, d_crateAcceleration, d_crateAngularVelocity, d_crateMaxVelocity, d_crateDrag,
            d_crateInteractRadius, false)
      , m_health(d_healthCrateHealth)
{
}
HealthCrate::HealthCrate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
        float interactRadius, bool used, int health)
    : Crate(position, size, rotation, renderLayer, texture, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag,
            interactRadius, used)
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

void HealthCrate::interact(std::shared_ptr<Player> player)
{
    if(m_used) return;
    // std::cout << "INTERACTION (C) -- " << player.getObjectID() << " | " << getObjectID() << std::endl;

	m_used = true;
    player->increaseHealth(m_health);
}
