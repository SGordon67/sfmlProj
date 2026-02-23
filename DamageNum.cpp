#include "BasicObject.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "globals.h"
#include <SFML/Graphics.hpp>

#include "DamageNum.h"
#include "VisualObject.h"

// use similar logic to entity marked for death

DamageNum::DamageNum(sf::Vector2f position, int damage)
    : VisualObject(position, {0, 0}, M_PI / 2, RenderLayer::Main, nullptr)
, m_damageText(sf::Text(font))
{
    m_xVelocity = ((float)rand() / RAND_MAX * 2.0f - 1.0f) * m_xVelocity;
}

sf::Text DamageNum::getText() { return m_damageText; }
sf::Color DamageNum::getColor() { return m_damageColor; }
float DamageNum::getYVelocity() { return m_yVelocity; }
float DamageNum::getGravity() { return m_gravity; }
float DamageNum::getXVelocity() { return m_xVelocity; }
float DamageNum::getXDecay() { return m_xDecay; }
float DamageNum::getTTL() { return m_ttl; }

void DamageNum::setYVelocity(float velocity) { m_yVelocity = velocity; }
void DamageNum::setXVelocity(float velocity) { m_xVelocity = velocity; }

void DamageNum::updateYVelocity()
{
    setYVelocity(getYVelocity() - getGravity());
}
void DamageNum::updateXVelocity()
{
    setXVelocity(getXVelocity() - getXDecay());
}

void DamageNum::visualUpdate(float deltaTime, sf::Vector2f relationalVelocity)
{
    basicUpdate(deltaTime, relationalVelocity);

    setPosition({getPosition().x + getXVelocity(), getPosition().y + getYVelocity()});
    updateYVelocity();
}
void DamageNum::draw(sf::RenderWindow& window)
{
    VisualObject::basicDraw(window);
}
