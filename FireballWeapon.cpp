#include "FireballWeapon.h"
#include "Entity.h"
#include "Player.h"
#include <algorithm>


// code to face towards a particular direction
        // // face toward the player
        // sf::Vector2f pPosition = m_player->getPosition();
        // sf::Vector2f delta = pPosition - m_position;
        //
        // // adjust for wrapping
        // if(std::abs(delta.x) > worldWidth / 2.0f)
        //     delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
        // if(std::abs(delta.y) > worldHeight / 2.0f)
        //     delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
        // float angle = std::atan2(delta.y, delta.x);
        //
        // setRotation(angle + M_PI);
        // m_sprite.setRotation(sf::radians(angle + M_PI/2));



extern bool detectIntersection(const sf::Vector2f& pos1, float size1, const sf::Vector2f& pos2, float size2);

// Weapon(int damage, float cooldown, float size, sf::Vector2f velocity, float duration, float kb , float timeSince, bool active)

FireballWeapon::FireballWeapon()
    : Weapon(20, 1, 20, {200, 0}, 2, 0, 0, false)
    , m_range(300.f)
{
    Weapon::setName("FireballWeapon");
}
FireballWeapon::FireballWeapon(int damage, float cooldown, float speed, float radius)
    : Weapon(damage, cooldown, radius, {speed, 0}, 2, 0, 0, false)
    , m_range(300.f)
{
    Weapon::setName("FireballWeapon");
}

void FireballWeapon::activate(Player& player, QuadTree& quadTree)
{
    Entity* nearestEntity = quadTree.getClosestEntity(player.getPosition(), m_range, &player);
    if(nearestEntity)
    {
        auto fireball = std::make_unique<Fireball>(player.getPosition(), nearestEntity->getPosition(),
                m_damage, m_speed, m_range);
        m_activeFireballs.push_back(std::move(fireball));
    }
}

void FireballWeapon::update(float deltaTime, Player& player, QuadTree& quadTree)
{

    Weapon::update(deltaTime, player, quadTree);

    for(auto& fireball : m_activeFireballs)
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
                fireball->dealDamage(entity);
                fireball->markForDestruction();
                break;
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
        // std::cout << "Rendering weapon, timer: " << m_visualTimer << std::endl;
        for(auto& fireball : m_activeFireballs)
        {
            fireball->draw(window);
        }
    }


