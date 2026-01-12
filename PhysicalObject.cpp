#include <iostream>

#include "PhysicalObject.h"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);
extern void detectAndHandleCollision(PhysicalObject& object, std::vector<PhysicalObject>& physicalObjects);
extern sf::Vector2f wrapPosition(sf::Vector2f position);

PhysicalObject::PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity)
	: BasicObject(position, size, rotation, renderLayer, filename)
	, m_mass(mass)
	, m_radius(radius)
	, m_velocity(velocity)
	, m_acceleration(acceleration)
	, m_angularVelocity(angularVelocity)
	, m_maxVelocity(maxVelocity)
{
}
PhysicalObject::PhysicalObject(const PhysicalObject& other) // copy constructor
	: BasicObject(other)
	, m_mass(other.m_mass)
	, m_radius(other.m_radius)
	, m_velocity(other.m_velocity)
	, m_acceleration(other.m_acceleration)
	, m_angularVelocity(other.m_angularVelocity)
	, m_maxVelocity(other.m_maxVelocity)
{
}
PhysicalObject::PhysicalObject(PhysicalObject&& other) noexcept // move constructor
	: BasicObject(std::move(other))
	, m_mass(other.m_mass)
	, m_radius(other.m_radius)
	, m_velocity(other.m_velocity)
	, m_acceleration(other.m_acceleration)
	, m_angularVelocity(other.m_angularVelocity)
	, m_maxVelocity(other.m_maxVelocity)
{
}

float PhysicalObject::getMass() const
{
	return m_mass;
}
void PhysicalObject::setMass(float mass)
{
	m_mass = mass;
}

float PhysicalObject::getRadius() const
{
	return m_radius;
}
void PhysicalObject::setRadius(float radius)
{
	m_radius = radius;
}

sf::Vector2f PhysicalObject::getVelocity() const
{
	return m_velocity;
}
void PhysicalObject::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
}

float PhysicalObject::getMaxVelocity() const
{
	return m_maxVelocity;
}
void PhysicalObject::setMaxVelocity(float maxVelocity)
{
	m_maxVelocity = maxVelocity;
}

float PhysicalObject::getAngularVelocity() const
{
	return m_angularVelocity;
}
void PhysicalObject::setAngularVelocity(float angularVelocity)
{
	m_angularVelocity = angularVelocity;
}

void PhysicalObject::rotate(const float rotation)
{
	m_sprite.rotate(sf::degrees(radiansToDegrees(rotation)));
	m_rotation += rotation;

	// wrap the rotation
	if(m_rotation >= 2 * M_PI) m_rotation -= 2 * M_PI;
	if(m_rotation < 0) m_rotation += 2 * M_PI;
}
void PhysicalObject::updateRotation()
{
	if(m_angularVelocity == 0) return;
	// std::cout << "Rotating by: " << m_angularVelocity * FixedDeltaTime << " radians (" << radiansToDegrees(m_angularVelocity * FixedDeltaTime) << " degrees)" << std::endl;
	PhysicalObject::rotate(m_angularVelocity * FixedDeltaTime);
	PhysicalObject::rotate(m_angularVelocity * FixedDeltaTime);
	m_angularVelocity *= angularDrag;

	// clamp small values
	if(std::abs(m_angularVelocity) < 0.01f)
	{
		m_angularVelocity = 0.0f;
	}
}

void PhysicalObject::updateVelocity(float accelerate, bool backward)
{
	// zero out the velocity if its small enough
	if(m_velocity.x < 0.5 && m_velocity.x > -0.5) m_velocity.x = 0;
	if(m_velocity.y < 0.5 && m_velocity.y > -0.5) m_velocity.y = 0;

	float direction = m_rotation;
	if(backward) direction += M_PI;

	addDragForce(m_velocity, m_mass, FixedDeltaTime);
	addAccelerationForce(m_velocity, accelerate, direction, backward, getMaxVelocity(), m_mass, FixedDeltaTime);

	// std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
	// std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
}

void PhysicalObject::updatePosition(float deltaTime)
{
	// ensure the change will work, adjust accordingly
	m_position += (m_velocity * deltaTime);
	m_position = wrapPosition(m_position);

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
			objPosition.x -= (getRadius());
			objPosition.y -= (getRadius());

			sf::CircleShape circle(m_radius);
			circle.setFillColor(hitboxColor);
			circle.setPosition(objPosition);
			window.draw(circle);
		}
	}
}

void PhysicalObject::physicalUpdate()
{
	float accel = 0;
	bool backward = false;
	updateRotation();
	updateVelocity(accel, backward);
	updatePosition(FixedDeltaTime);
}
