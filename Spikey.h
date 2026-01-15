#ifndef SPIKEY_H
#define SPIKEY_H

#include "SFML/System/Vector2.hpp"

#include "Entity.h"

class Spikey: public Entity
{
private:
	int m_damage;
public:
	Spikey(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
	       float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
	       int hp, int maxHP, int damage);
	Spikey(const Spikey& other); // copy constructor
	Spikey(Spikey&& other) noexcept; // move constructor
					 //
	int getDamage() const;
	void setDamage(int damage);
};

#endif
