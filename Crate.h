#ifndef CRATE_H
#define CRATE_H

#include "PhysicalObject.h"

class Crate : public PhysicalObject
{
protected:
	float m_interactRadius;
	bool m_used;
public:
	Crate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
		float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
		float interactRadius, bool used)
        : PhysicalObject(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity)
          , m_interactRadius(interactRadius)
          , m_used(used)
    {
    }
    Crate(const Crate& other) // copy constructor
        : PhysicalObject(other)
          , m_interactRadius(other.m_interactRadius)
          , m_used(other.m_used)
    {
    }
    Crate(Crate&& other) noexcept // copy constructor
        : PhysicalObject(std::move(other))
          , m_interactRadius(other.m_interactRadius)
          , m_used(other.m_used)
    {
    }

    float getInteractionRadius()
    {
        return m_interactRadius;
    }
    void setInteractionRadius(float radius)
    {
        m_interactRadius = radius;
    }

    bool getUsed()
    {
        return m_used;
    }
    void setUsed(bool used)
    {
        m_used = used;
    }
};

#endif
