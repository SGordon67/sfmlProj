#include "Spikey.h"
#include "PhysicalObject.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

Spikey::Spikey()
    : Entity(sf::Vector2f(0.1, 0.1), sf::Vector2i(20, 20), M_PI/2, RenderLayer::Main, &spikeyTexture, 
            10, 8, sf::Vector2f(0, 0), 0, 0, 500,
            100, 100)
{
}
Spikey::Spikey(sf::Vector2f position)
    : Entity(position, sf::Vector2i(20, 20), M_PI/2, RenderLayer::Main, &spikeyTexture, 
            10, 8, sf::Vector2f(0, 0), 0, 0, 500,
            100, 100)
{
}
Spikey::Spikey(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
	       float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
	       int hp, int maxHP, int damage)
	: Entity(position, size, rotation, renderLayer, texture, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
	, m_damage(damage)
{
}
Spikey::Spikey(const Spikey& other) // copy constructor
	: Entity(other)
	, m_damage(other.getDamage())
{
}
Spikey::Spikey(Spikey&& other) noexcept // move constructor
	: Entity(std::move(other))
	, m_damage(other.getDamage())
{
}
int Spikey::getDamage() const
{
	return m_damage;
}
void Spikey::setDamage(int damage)
{
	m_damage = damage;
}

// hazardous interface
int Spikey::getObjectID() const
{
    return BasicObject::getObjectID();
}
sf::Vector2f Spikey::getPosition() const
{
    return BasicObject::getPosition();
}
float Spikey::getRadius() const
{
    return PhysicalObject::getRadius();
}
void Spikey::dealDamage(std::shared_ptr<Entity> entity)
{
    entity->reduceHealth(getDamage());
}
void Spikey::update()
{
    std::cout << "Updating Spikey: " << getObjectID() << std::endl;
}
