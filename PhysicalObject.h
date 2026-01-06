#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include "VisualObject.h"

class PhysicalObject : public VisualObject
{
protected:
	float m_mass;
	float m_radius; // for collisions

	sf::Vector2f m_velocity;
	float m_acceleration;
	float m_dragCoef;

	float m_rotationVelocity;

	float m_maxVelocity;

public:
	PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity);

	float getMass() const;
	float getRadius() const;
	sf::Vector2f getVelocity() const;
	float getMaxVelocity() const;

	void rotate(const float rotation);

	void updateVelocity(float accelerate, bool backward);
	void updatePosition(float deltaTime, std::vector<PhysicalObject>& physicalObjects);

	virtual void update(std::vector<PhysicalObject>& physicalObjects);
};

#endif
