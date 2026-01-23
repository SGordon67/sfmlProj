#ifndef ENEMY1_H
#define ENEMY1_H

#include "SFML/System/Vector2.hpp"

#include "Player.h"
#include "Enemy.h"

class Enemy1 : public Enemy
{
private:
public:
	Enemy1(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
	       float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
	       int hp, int maxHP, int damage, std::shared_ptr<Player> player);
	Enemy1(const Enemy1& other); // copy constructor
	Enemy1(Enemy1&& other) noexcept; // move constructor
};

#endif
