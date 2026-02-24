#include "globals.h"
#include <SFML/Graphics.hpp>

#include "DamageNum.h"
#include "VisualObject.h"

// use similar logic to entity marked for death

DamageNum::DamageNum(sf::Vector2f position, int damage)
    : VisualObject(position, {0, 0}, M_PI / 2, RenderLayer::Main, nullptr)
, m_damageText(sf::Text(font))
{
    m_damageText.setString(std::to_string(damage));
    m_damageText.setPosition(position);
    m_damageText.setCharacterSize(m_damageTextSize);
    m_damageText.setFillColor(m_damageColor);
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
    setYVelocity(getYVelocity() + getGravity());
}
void DamageNum::updateXVelocity()
{
    setXVelocity(getXVelocity() - getXDecay());
}

void DamageNum::decrementTTL()
{
    m_ttl--;
}

void DamageNum::visualUpdate(float deltaTime, sf::Vector2f relationalVelocity)
{
    basicUpdate(deltaTime, relationalVelocity);

    setPosition({getPosition().x + getXVelocity(), getPosition().y + getYVelocity()});
    updateYVelocity();
    decrementTTL();
    if(getTTL() == 0) markForDelete();
}
void DamageNum::draw(sf::RenderWindow& window)
{
    sf::View wView = window.getView();
    sf::Vector2f viewCenter = wView.getCenter();
    sf::Vector2f viewSize = wView.getSize();

    auto isOnScreen = [&](const sf::Vector2f& pos)
    {
        float dx = std::abs(pos.x - viewCenter.x);
        float dy = std::abs(pos.y - viewCenter.y);
        return (dx < viewSize.x * 0.6f && dy < viewSize.y * 0.6f);
    };

    sf::Vector2f myClosestPosition = getClosestWrapPosition(viewCenter, getPosition());
    if(isOnScreen(myClosestPosition))
    {
        m_damageText.setPosition(myClosestPosition);
        window.draw(m_damageText);
        m_damageText.setPosition(m_position);
    }
}
