#include "enums.h"
#include "PhysicalObject.h"

PhysicalObject::PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity)
	: VisualObject(position, size, rotation, renderLayer, filename)
	, m_mass(mass)
	, m_radius(radius)
	, m_velocity(velocity)
	, m_acceleration(acceleration)
	, m_dragCoef(dragCoef)
		, m_rotationVelocity(rotationVelocity)
{
	m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});
	// std::cout << "PhysicalObject constructed" << std::endl;
}

sf::Vector2f PhysicalObject::getVelocity() const
{
	return m_velocity;
}

void PhysicalObject::updatePosition(float deltaTime)
{
	m_position += m_velocity * deltaTime;
	m_sprite.setPosition(m_position);
}

void PhysicalObject::updateVelocity(float accelerate)
{

}

void PhysicalObject::update()
{
	std::cout << "Updating Physical Object" << std::endl;
}
