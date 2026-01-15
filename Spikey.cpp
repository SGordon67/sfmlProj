#include "Spikey.h"

Spikey::Spikey(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
	       float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
	       int hp, int maxHP, int damage)
	: Entity(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
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
