#ifndef FIREBALL_H
#define FIREBALL_H

#include "PhysicalObject.h"
#include "Player.h"
#include "QuadTree.h"
#include <SFML/Graphics.hpp>

class Fireball : public PhysicalObject {
private:
    int m_damage = 20;
    float m_expRadius = 50;
    float m_timeToLive = 1;
    bool m_hasHit = false;

    float m_expVisualTime = 0.5;

public:
    Fireball(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float speed, float range);

    int getDamage() const { return m_damage; }
    float getExpRadius() const { return m_expRadius; }
    float getTTL() const { return m_timeToLive; }
    float getExpVisualTime() const { return m_expVisualTime; }

    void setExpVisualTime(float timeLeft) { m_expVisualTime = timeLeft; }

    void update() override;
    void draw(sf::RenderWindow& window) override;

    void dealDamage(QuadTree& quadTree, Player& player);
    bool shouldBeDestroyed() const;
    void markForDestruction();
    bool hasHit() const { return m_hasHit; }
};

#endif
