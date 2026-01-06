#include "globals.h"
#include "Player.h"
#include "PhysicalObject.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

Player::Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity)
	: PhysicalObject(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, dragCoef, rotationVelocity, maxVelocity)
{
	m_sprite.setOrigin({14.5, 18}); // roughtly the point of rotation I want
	std::cout << "Player constructed" << std::endl;
}

void Player::printInfo()
{
	std::cout << std::endl << "Player Info: " << std::endl;
	std::cout << "Position: (" << m_position.x << ", " << m_position.y << ")" << std::endl;
	std::cout << "Velocity: (" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
}



void Player::update(bool* (&buttons)[numButtons], std::vector<PhysicalObject>& physicalObjects)
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
	this->updatePosition(FixedDeltaTime, physicalObjects);

	// this->printInfo();
}
