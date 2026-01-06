#include "SFML/System/Vector2.hpp"
#include "enums.h"
#include "globals.h"
#include "PhysicalObject.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);
extern void detectCollision(PhysicalObject& object, sf::Vector2f newPosition, std::vector<PhysicalObject>& physicalObjects);

PhysicalObject::PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity)
	: VisualObject(position, size, rotation, renderLayer, filename)
	, m_mass(mass)
	, m_radius(radius)
	, m_velocity(velocity)
	, m_acceleration(acceleration)
	, m_dragCoef(dragCoef)
	, m_rotationVelocity(rotationVelocity)
	, m_maxVelocity(maxVelocity)
{
	m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});
	// std::cout << "PhysicalObject constructed" << std::endl;
}

float PhysicalObject::getMass() const
{
	return m_mass;
}

float PhysicalObject::getRadius() const
{
	return m_radius;
}

sf::Vector2f PhysicalObject::getVelocity() const
{
	return m_velocity;
}

float PhysicalObject::getMaxVelocity() const
{
	return m_maxVelocity;
}

void PhysicalObject::rotate(const float rotation)
{
	// std::cout << "Rotate Ojbect by: " << rotation << " degrees" << std::endl;
	m_sprite.rotate(sf::degrees(rotation));
	m_rotation -= rotation;
	if(m_rotation >= 360) m_rotation -= 360;
	if(m_rotation < 0) m_rotation += 360;
}

void PhysicalObject::updateVelocity(float accelerate, bool backward)
{
	// zero out the velocity if its small enough
	if(m_velocity.x < 0.5 && m_velocity.x > -0.5) m_velocity.x = 0;
	if(m_velocity.y < 0.5 && m_velocity.y > -0.5) m_velocity.y = 0;

	float direction = m_rotation;
	if(backward) direction += 180;
	direction = degreesToRadians(direction);

	addDragForce(m_velocity, m_dragCoef, m_mass, FixedDeltaTime);
	addAccelerationForce(m_velocity, accelerate, direction, backward, getMaxVelocity(), m_mass, FixedDeltaTime);

	// std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
	// std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
}

void PhysicalObject::updatePosition(float deltaTime, std::vector<PhysicalObject>& physicalObjects)
{
	// ensure the change will work, adjust accordingly
	sf::Vector2f newPosition = m_position + m_velocity * deltaTime;
	detectCollision(*this, newPosition, physicalObjects);

	// apply the change
	m_position = newPosition;
	m_sprite.setPosition(m_position);
}

void PhysicalObject::update(std::vector<PhysicalObject>& physicalObjects)
{
	float accel = 0;
	bool backward = false;
	this->updateVelocity(accel, backward);
	this->updatePosition(FixedDeltaTime, physicalObjects);
}
