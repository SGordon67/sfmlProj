#ifndef ENTITY_H
#define ENTITY_H

#include "PhysicalObject.h"

class Entity : public PhysicalObject
{
protected:
	int m_hp;
	int m_maxHP;
public:

	Entity(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, int hp, int maxHP)
			: PhysicalObject(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity)
			  , m_hp(hp)
			  , m_maxHP(maxHP)
	{
	}

	int getHP() const
	{
		return m_hp;
	}

	int getMaxHP() const
	{
		return m_maxHP;
	}

	void reduceHealth(int damage)
	{
		m_hp -= damage;
	}

	void increaseHealth(int heal)
	{
		m_hp += heal;
		if(m_hp > m_maxHP) m_hp = m_maxHP;
	}
};

#endif
