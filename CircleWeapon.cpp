#include "CircleWeapon.h"
#include "Entity.h"
#include "Player.h"
#include <algorithm>
#include <memory>

CircleWeapon::CircleWeapon()
    : Weapon(10, 0.25, 0, false)
      , m_radius(100)
{
}
CircleWeapon::CircleWeapon(int damage, float cooldown, float radius)
    : Weapon(damage, cooldown, 0, false)
      , m_radius(radius)
{
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
      , m_radius(other.m_radius)
{
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
    : Weapon(std::move(other))
      , m_radius(other.m_radius)
{
}



void CircleWeapon::activate(Player& player, QuadTree& quadTree)
{
    std::vector<std::shared_ptr<PhysicalObject>> nearby;
    quadTree.retrieveToroidal(nearby, player.getPosition(), m_radius);

    for(auto& obj : nearby)
    {
        if(obj.get() == &player) continue;

        sf::Vector2f diff = obj->getPosition() - player.getPosition();
        float distSqr = diff.x * diff.x + diff.y * diff.y;

        if(distSqr <= m_radius * m_radius)
        {
            auto entity = std::dynamic_pointer_cast<Entity>(obj);
            if(entity)
            {
                entity->reduceHealth(m_damage);
            }
        }
    }
}
