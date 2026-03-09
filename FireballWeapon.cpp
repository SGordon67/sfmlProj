#include "FireballWeapon.h"
#include "BasicObject.h"
#include "Entity.h"
#include "Player.h"
#include "SFML/Graphics/Color.hpp"
#include "globals.h"
#include <algorithm>

        // static constexpr sf::Vector2i d_fireballWeaponSize = {20, 20};
        // static constexpr int d_fireballWeaponMass = 0;
        // static constexpr float d_fireballWeaponRadius = 10;
        // static constexpr sf::Vector2f d_fireballWeaponVelocity = {0, 0};
        // static constexpr float d_fireballWeaponAcceleration = 0;
        // static constexpr float d_fireballWeaponAngularVelocity = 0;
        // static constexpr float d_fireballWeaponMaxVelocity = 200;
        // static constexpr float d_fireballWeaponDrag = 0;


extern bool detectIntersection(const sf::Vector2f& pos1, float size1, const sf::Vector2f& pos2, float size2);

// Weapon(int damage, float cooldown, float size, sf::Vector2f velocity, float duration, float kb , float timeSince, bool active)

FireballWeapon::FireballWeapon()
    : Weapon(d_damage, d_cooldown, d_size, {0, 0}, d_duration, d_kb, 999999, false)
    , m_range(d_range)
    , m_fbRange(d_fbRange)
{
    Weapon::setName("FireballWeapon");
}
FireballWeapon::FireballWeapon(int damage, float cooldown, float speed, float radius)
    : Weapon(damage, cooldown, radius, {speed, 0}, 2, 0, 0, false)
    , m_range(d_range)
    , m_fbRange(d_fbRange)
{
    Weapon::setName("FireballWeapon");
}

float FireballWeapon::getRange()
{
    return m_range;
}
sf::Vector2f FireballWeapon::getPosition()
{
    return m_position;
}

void FireballWeapon::setRange(float range)
{
    m_range = range;
}
void FireballWeapon::setPosition(sf::Vector2f pos)
{
    m_position = pos;
}

void FireballWeapon::activate(Player& player, QuadTree& quadTree)
{
    Entity* nearestEntity = quadTree.getClosestEntity(player.getPosition(), m_size, &player);
    if(nearestEntity)
    {
        sf::Vector2f nearestPosition = getClosestWrapPosition(player.getPosition(), nearestEntity->getPosition());
        double dx = nearestPosition.x - player.getPosition().x;
        double dy = nearestPosition.y - player.getPosition().y;
        double dist = std::sqrt(dx * dx + dy * dy);

        if(dist <= m_range)
        {
            auto fireball = std::make_unique<Fireball>(player.getPosition(), nearestEntity->getPosition(),
                    m_damage, d_fireballWeaponMaxVelocity, m_fbRange);
            m_activeFireballs.push_back(std::move(fireball));
        }
    }
}

void FireballWeapon::update(float deltaTime, Player& player, QuadTree& quadTree)
{
    setPosition(player.getPosition());
    Weapon::update(deltaTime, player, quadTree);

    for(auto& fireball : m_activeFireballs)
    {
        if(!fireball->hasHit())
        {
            fireball->update();

            std::vector<Entity*> nearbyEntities;
            quadTree.retrieveEntities(nearbyEntities, fireball->getPosition(), fireball->getRadius() + 10);

            for(auto* entity : nearbyEntities)
            {
                if(dynamic_cast<Player*>(entity))
                    continue;

                if(fireball->hasHit())
                    continue;

                bool overlap = detectIntersection(fireball->getPosition(), fireball->getRadius(),
                        entity->getPosition(), entity->getRadius());

                if(overlap)
                {
                    fireball->setVelocity({0, 0});
                    fireball->dealDamage(quadTree, player);
                    fireball->markForDestruction();
                    break;
                }
            }
        }
    }
    m_activeFireballs.erase(
            std::remove_if(m_activeFireballs.begin(), m_activeFireballs.end(), 
                [](const auto& fireball)
                {
                    return fireball->shouldBeDestroyed();
                }),
            m_activeFireballs.end());
}

void FireballWeapon::render(sf::RenderWindow& window)
{
    for(auto& fireball : m_activeFireballs)
    {
        if(!fireball->hasHit())
        {
            fireball->draw(window);
        }
        else
        {
            fireball->setExpVisualTime(fireball->getExpVisualTime() - FixedDeltaTime);
            sf::CircleShape circle(fireball->getExpRadius());
            circle.setOrigin(sf::Vector2f(fireball->getExpRadius(), fireball->getExpRadius()));
            circle.setFillColor(sf::Color(255, 0, 0, 100));
            circle.setPosition(getClosestWrapPosition(getPosition(), fireball->getPosition()));
            window.draw(circle);
        }
    }
}
