#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include "BasicObject.h"

class PhysicalObject : public BasicObject
{
protected:
	float m_mass;
	float m_radius; // for collisions

	sf::Vector2f m_velocity;
	float m_acceleration;

	float m_angularVelocity;

	float m_maxVelocity;

public:
	PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity);
	PhysicalObject(const PhysicalObject& other); // copy constructor
	PhysicalObject(PhysicalObject&& other) noexcept; // move constructor

	float getMass() const;
	void setMass(float mass);
	float getRadius() const;
	void setRadius(float radius);
	sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f velocity);
	float getAcceleration() const;
	void setAcceleration(float acceleration);
	float getMaxVelocity() const;
	void setMaxVelocity(float maxVelocity);
	float getAngularVelocity() const;
	void setAngularVelocity(float angularVelocity);

	virtual void rotate(const float rotation);
	virtual void updateRotation();
	virtual void updateVelocity(float accelerate, bool backward);
	virtual void updatePosition(float deltaTime);

	virtual void physicalDraw(sf::RenderWindow& window);
	virtual void physicalUpdate();
};

#endif
