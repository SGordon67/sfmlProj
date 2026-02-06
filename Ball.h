#ifndef BALL_H
#define BALL_H

#include "PhysicalObject.h"

class Ball : public PhysicalObject
{
private:
    // defaults
    static constexpr sf::Vector2i d_ballSize = {17, 17};
    static constexpr int d_ballMass = 10;
    static constexpr float d_ballRadius = 8;
    static constexpr sf::Vector2f d_ballVelocity = {0, 0};
    static constexpr float d_ballAcceleration = 0;
    static constexpr float d_ballAngularVelocity = 0;
    static constexpr float d_ballMaxVelocity = 500;
    static constexpr float d_ballDrag = 4;

public:
    Ball();
    Ball(sf::Vector2f position);
	Ball(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity, float drag);
	Ball(const Ball& other); // copy constructor
	Ball(Ball&& other) noexcept; // move constructor
};

#endif
