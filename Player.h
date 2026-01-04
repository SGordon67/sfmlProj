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
	float m_maxVelocity;
public:
	Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, float mass, float radius, sf::Vector2f velocity, float acceleration, float dragCoef, float rotationVelocity, float maxVelocity);
	float getMaxVelocity();
	void rotate(const float rotation) override;
	void updateVelocity(float accelerate, bool backward);
	void update(bool* (&buttons)[numButtons]);
};

#endif
