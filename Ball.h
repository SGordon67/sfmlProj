#ifndef BALL_H
#define BALL_H

#include "PhysicalObject.h"

class Ball : public PhysicalObject
{
private:
public:
    Ball();
    Ball(sf::Vector2f position);
	Ball(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity);
	Ball(const Ball& other); // copy constructor
	Ball(Ball&& other) noexcept; // move constructor
};

#endif
