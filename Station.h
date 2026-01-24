#ifndef STATION_H
#define STATION_H

#include "PhysicalObject.h"

class Station : public PhysicalObject
{
protected:
	bool m_used;
public:

	Station(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
		float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
		int hp, int maxHP, bool used)
			: PhysicalObject(position, size, rotation, renderLayer, texture, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity)
			  , m_used(used)
	{
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
