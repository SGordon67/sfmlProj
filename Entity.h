#ifndef ENTITY_H
#define ENTITY_H

#include "PhysicalObject.h"

class Entity : public PhysicalObject
{
protected:
	int m_hp;
	int m_maxHP;
public:
	Entity(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity, int hp, int maxHP);

	int getHP() const;
	int getMaxHP() const;

	void reduceHealth(int damage);
	void increaseHealth(int heal);
};

#endif
