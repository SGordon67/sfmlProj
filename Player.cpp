#include "globals.h"
#include "Player.h"
#include "PhysicalObject.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

Player::Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity)
	: PhysicalObject(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, dragCoef, rotationVelocity)
		, m_maxVelocity(maxVelocity)
{
	m_sprite.setOrigin({21, 25}); // roughtly the point of rotation I want
	std::cout << "Player constructed" << std::endl;
}

float Player::getMaxVelocity()
{
	return m_maxVelocity;
}

void Player::rotate(const float rotation)
{
	// std::cout << "Rotation player by: " << rotation << " degrees" << std::endl;
	m_sprite.rotate(sf::degrees(rotation));
	m_rotation -= rotation;
	if(m_rotation >= 360) m_rotation -= 360;
	if(m_rotation < 0) m_rotation += 360;
}

void Player::updateVelocity(float accelerate, bool backward) 
{
	// zero out the velocity if its small enough
	if(m_velocity.x < 0.5 && m_velocity.x > -0.5) m_velocity.x = 0;
	if(m_velocity.y < 0.5 && m_velocity.y > -0.5) m_velocity.y = 0;

	float direction = m_rotation;
	if(backward) direction += 180;
	direction = degreesToRadians(direction);

	addDragForce(m_velocity, m_dragCoef, m_mass, FixedDeltaTime);
	addAccelerationForce(m_velocity, accelerate, direction, backward, getMaxVelocity(), m_mass, FixedDeltaTime);

	std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
	std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
	std::cout << std::endl;
}

void Player::update(bool* (&buttons)[numButtons])
{
	// handle rotation
	float potentialRotation = 0;
	if(*buttons[3]) // left
	{
		potentialRotation -= m_rotationVelocity;
	}
	if(*buttons[4]) // right
	{
		potentialRotation += m_rotationVelocity;
	}
	if(potentialRotation != 0) 
	{
		this->rotate(potentialRotation);
	}

	// handle acceleration
	sf::Vector2f curVelocity = m_velocity;
	int accel = 0;
	bool backward = false;
	if(*buttons[1] && !*buttons[2])
	{
		accel = m_acceleration;
	} else if(!*buttons[1] && *buttons[2])
	{
		backward = true;
		accel = -m_acceleration;
	}
	this->updateVelocity(accel, backward);
	this->updatePosition(FixedDeltaTime);
}
