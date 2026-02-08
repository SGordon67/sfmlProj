#ifndef ENTITY_H
#define ENTITY_H

#include "PhysicalObject.h"
#include <string>

class Entity : public PhysicalObject
{
protected:
	int m_hp;
	int m_maxHP;
    bool m_markedForDeath;
public:

	Entity(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
            int hp, int maxHP)
			: PhysicalObject(position, size, rotation, renderLayer, texture, 
                    mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag)
			  , m_hp(hp)
			  , m_maxHP(maxHP)
              , m_markedForDeath(false)
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
    void markForDeath()
    {
        m_markedForDeath = true;
    }
    bool isMarkedForDeath()
    {
        return m_markedForDeath;
    }

	void reduceHealth(int damage)
	{
        std::string output = "Entity (" + std::to_string(getObjectID()) + ") damaged, (" + std::to_string(getHP()) + " -> ";
        setHP(getHP() - damage);
        if(getHP() <= 0)
        {
            m_hp = 0;
            markForDeath();
        }
        output += std::to_string(getHP()) + ")";

        std::cout << output << std::endl;
	}

	void increaseHealth(int heal)
	{
		m_hp += heal;
		if(m_hp > m_maxHP) m_hp = m_maxHP;
        std::cout << "HEALED, NEW HP -- " << getHP() << std::endl;
	}
};

#endif
