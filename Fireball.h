#ifndef FIREBALL_H
#define FIREBALL_H

#include "Entity.h"
#include "PhysicalObject.h"
#include <SFML/Graphics.hpp>

class Fireball : public PhysicalObject {
private:
    int m_damage = 20;
    float m_timeToLive = 1;
    bool m_hasHit = false;

public:
    Fireball(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float speed, float range);

    void update() override;
    void draw(sf::RenderWindow& window) override;

    void dealDamage(Entity* entity);
    bool shouldBeDestroyed() const;
    void markForDestruction();
    bool hasHit() const { return m_hasHit; }
};

#endif
