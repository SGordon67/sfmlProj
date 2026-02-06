#ifndef ENTITY_H
#define ENTITY_H

#include "PhysicalObject.h"

class Entity : public PhysicalObject
{
protected:
	int m_hp;
	int m_maxHP;
public:

	Entity(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
            int hp, int maxHP)
			: PhysicalObject(position, size, rotation, renderLayer, texture, 
                    mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag)
			  , m_hp(hp)
			  , m_maxHP(maxHP)
	{
	}

	int getHP() const
	{
		return m_hp;
	}
    void setHP(int hp)
    {
        m_hp = hp;
    }

	int getMaxHP() const
	{
		return m_maxHP;
	}
    void setMaxHP(int hp)
    {
        m_maxHP = hp;
    }

	void reduceHealth(int damage)
	{
        setHP(getHP() - damage);
        // std::cout << "DAMAGED, NEW HP -- " << getHP() << std::endl;
	}

	void increaseHealth(int heal)
	{
		m_hp += heal;
		if(m_hp > m_maxHP) m_hp = m_maxHP;
        std::cout << "HEALED, NEW HP -- " << getHP() << std::endl;
	}
};

#endif
