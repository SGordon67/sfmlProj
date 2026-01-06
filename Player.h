#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/System/Vector2.hpp"
#include <cmath>

#include "globals.h"
#include "enums.h"
#include "PhysicalObject.h"

class Player : public PhysicalObject
{
private:
public:
	Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity);
	void printInfo();
	void update(bool* (&buttons)[numButtons], std::vector<PhysicalObject>& physicalObjects);
};

#endif
