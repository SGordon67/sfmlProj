#ifndef DAMAGENUM_H
#define DAMAGENUM_H

#include "VisualObject.h"

// texture object is wasted in this class for now, 
// would require a bit of a refactor to take that out of basicobject at this point
class DamageNum : public VisualObject
{
protected:
    sf::Text m_damageText;
    sf::Color m_damageColor = sf::Color::Yellow;
    float m_damageTextSize = 9;

    float m_yVelocity = -1;
    float m_gravity = 0.067; // ~ 1 / (ttl / 2) (half the time up/down)
    float m_xVelocity = 0.4;
    float m_xDecay = 0.05;

    float m_ttl = 30; // time to live

public:
	DamageNum(sf::Vector2f position, int damage);

    sf::Text getText();
    sf::Color getColor();
    float getYVelocity();
    float getGravity();
    float getXVelocity();
    float getXDecay();
    float getTTL();

    void setYVelocity(float velocity);
    void setXVelocity(float velocity);

    void decrementTTL();

    void updateYVelocity();
    void updateXVelocity();

	void visualUpdate(float deltaTime, sf::Vector2f relationalVelocity) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
