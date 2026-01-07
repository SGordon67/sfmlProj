#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "enums.h"
#include "globals.h"
#include "PhysicalObject.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);
extern void detectAndHandleCollision(PhysicalObject& object, std::vector<PhysicalObject>& physicalObjects, float deltaTime);
extern sf::Vector2f wrapPosition(sf::Vector2f position);

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

void PhysicalObject::setVelocity(sf::Vector2f velocity)
{
	m_velocity = velocity;
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
	m_position += (m_velocity * deltaTime);
	m_position = wrapPosition(m_position);
	detectAndHandleCollision(*this, physicalObjects, deltaTime);

	// apply the change to the sprite
	m_sprite.setPosition(m_position);
}

void PhysicalObject::draw(sf::RenderWindow& window)
{
	// main object
	window.draw(m_sprite);
	sf::Vector2f originalPosition = m_position;

	// need to replicate near the boundaries
	bool nearLeft = (m_position.x - (viewWidth / 2.f) < 0);
	bool nearRight = (m_position.x + (viewWidth / 2.f) > worldWidth);
	bool nearTop = (m_position.y - (viewHeight / 2.f) < 0);
	bool nearBottom = (m_position.y + (viewHeight / 2.f) > worldHeight);
	if(nearLeft)
	{
		setPosition(m_position + sf::Vector2f{static_cast<float>(viewWidth), 0});
		window.draw(m_sprite);
		if(nearTop)
		{
			setPosition(m_position + sf::Vector2f{0, static_cast<float>(viewHeight)});
			window.draw(m_sprite);
		}
		if(nearBottom)
		{
			setPosition(m_position - sf::Vector2f{0, static_cast<float>(viewHeight)});
			window.draw(m_sprite);
		}
	}
	if(nearRight)
	{
		setPosition(m_position - sf::Vector2f{static_cast<float>(viewWidth), 0});
		window.draw(m_sprite);
		if(nearTop)
		{
			setPosition(m_position + sf::Vector2f{0, static_cast<float>(viewHeight)});
			window.draw(m_sprite);
		}
		if(nearBottom)
		{
			setPosition(m_position - sf::Vector2f{0, static_cast<float>(viewHeight)});
			window.draw(m_sprite);
		}
	}
	setPosition(originalPosition);
	if(nearTop)
	{
		setPosition(m_position + sf::Vector2f{0, static_cast<float>(viewHeight)});
		window.draw(m_sprite);
	}
	if(nearBottom)
	{
		setPosition(m_position - sf::Vector2f{0, static_cast<float>(viewHeight)});
		window.draw(m_sprite);
	}
	setPosition(originalPosition);

	// draw the shapes used for collision
	if(showHitboxes)
	{
		// get the offset coords to be centered
		sf::Vector2f objPosition = m_position;
		objPosition.x -= (m_size.x / 2.f);
		objPosition.y -= (m_size.y / 2.f);

		sf::CircleShape circle(m_radius);
		circle.setFillColor(hitboxColor);
		circle.setPosition(objPosition);
		window.draw(circle);
	}
}

void PhysicalObject::update(std::vector<PhysicalObject>& physicalObjects)
{
	float accel = 0;
	bool backward = false;
	this->updateVelocity(accel, backward);
	this->updatePosition(FixedDeltaTime, physicalObjects);
}
