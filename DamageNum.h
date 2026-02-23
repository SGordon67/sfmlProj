#ifndef DAMAGENUM_H
#define DAMAGENUM_H

#include "VisualObject.h"

class DamageNum : public VisualObject
{
protected:
    sf::Text damageText;
    sf::Color damageColor = sf::Color::Red;
    sf::Vector2f position = {0, 0};

    float gravity = 5;
    float initialYVelocity = 1;
    float xVelocityMax = 1;

public:
	DamageNum(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture);
	DamageNum(const DamageNum& other); // copy constructor
	DamageNum(DamageNum&& other) noexcept; // move constructor

	virtual void visualUpdate(float deltaTime, sf::Vector2f relationalVelocity);
};

#endif
