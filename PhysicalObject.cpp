#include <iostream>

#include "PhysicalObject.h"
#include "SFML/System/Vector2.hpp"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);
extern void detectAndHandleCollision(PhysicalObject& object, std::vector<PhysicalObject>& physicalObjects);
extern sf::Vector2f wrapPosition(sf::Vector2f position);

PhysicalObject::PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity)
	: BasicObject(position, size, rotation, renderLayer, filename)
	, m_mass(mass)
	, m_radius(radius)
	, m_velocity(velocity)
	, m_acceleration(acceleration)
	, m_dragCoef(dragCoef)
	, m_rotationVelocity(rotationVelocity)
	, m_maxVelocity(maxVelocity)
{
}
PhysicalObject::PhysicalObject(const PhysicalObject& other) // copy constructor
	: BasicObject(other)
	, m_mass(other.m_mass)
	, m_radius(other.m_radius)
	, m_velocity(other.m_velocity)
	, m_acceleration(other.m_acceleration)
	, m_dragCoef(other.m_dragCoef)
	, m_rotationVelocity(other.m_rotationVelocity)
	, m_maxVelocity(other.m_maxVelocity)
{
}
PhysicalObject::PhysicalObject(PhysicalObject&& other) noexcept // move constructor
	: BasicObject(std::move(other))
	, m_mass(other.m_mass)
	, m_radius(other.m_radius)
	, m_velocity(other.m_velocity)
	, m_acceleration(other.m_acceleration)
	, m_dragCoef(other.m_dragCoef)
	, m_rotationVelocity(other.m_rotationVelocity)
	, m_maxVelocity(other.m_maxVelocity)
{
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

void PhysicalObject::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

void PhysicalObject::rotate(const float rotation)
{
	// std::cout << "Rotate Object by: " << rotation << " degrees" << std::endl;
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
	m_position += (m_velocity * deltaTime);
	m_position = wrapPosition(m_position);
	detectAndHandleCollision(*this, physicalObjects);

	// apply the change to the sprite
	m_sprite.setPosition(m_position);
}

void PhysicalObject::physicalDraw(sf::RenderWindow& window)
{
	// main object
	std::vector<sf::Vector2f> dupPositions;
	dupPositions = basicDraw(window);

	// adding starting position for hitbox
	dupPositions.push_back(m_position);

	// draw the shapes used for collision
	if(showHitboxes)
	{
		for(const auto& pos : dupPositions)
		{
			// get the offset coords to be centered
			sf::Vector2f objPosition = pos;
			objPosition.x -= (m_size.x / 2.f);
			objPosition.y -= (m_size.y / 2.f);

			sf::CircleShape circle(m_radius);
			circle.setFillColor(hitboxColor);
			circle.setPosition(objPosition);
			window.draw(circle);
		}
	}
}

void PhysicalObject::physicalUpdate(std::vector<PhysicalObject>& physicalObjects)
{
	float accel = 0;
	bool backward = false;
	this->updateVelocity(accel, backward);
	this->updatePosition(FixedDeltaTime, physicalObjects);
}
