#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
private:
public:
	Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity, int hp, int maxHP);

	void printInfo();
	void playerUpdate(bool* (&buttons)[numButtons]);
};

#endif
