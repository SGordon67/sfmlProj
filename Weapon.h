#ifndef WEAPON_H
#define WEAPON_H

#include "QuadTree.h"
#include <iostream>

class Player;

class Weapon
{
    protected:
        std::string m_name;
        int m_damage;
        float m_cooldown;
        float m_timeSinceLastFire;
        bool m_active;

    public:
        Weapon()
            : m_name("Weapon")
              , m_damage(10)
              , m_cooldown(1)
              , m_timeSinceLastFire(m_cooldown)
              , m_active(false)
    {
    }
        Weapon(int damage, float cooldown, float timeSince, bool active)
            : m_name("Weapon")
              , m_damage(damage)
              , m_cooldown(cooldown)
              , m_timeSinceLastFire(timeSince)
              , m_active(active)
    {
    }
        Weapon(const Weapon& other) // copy constructor
            : m_name("Weapon")
              , m_damage(other.m_damage)
              , m_cooldown(other.m_cooldown)
              , m_timeSinceLastFire(other.m_timeSinceLastFire)
              , m_active(other.m_active)
    {
    }
        Weapon(Weapon&& other) noexcept // move constructor
            : m_name("Weapon")
            , m_damage(other.m_damage)
            , m_cooldown(other.m_cooldown)
            , m_timeSinceLastFire(other.m_timeSinceLastFire)
            , m_active(other.m_active)
            {
            }

        virtual ~Weapon() = default;

        void setName(std::string name)
        {
            m_name = name;
        }
        std::string getName()
        {
            return m_name;
        }
        void setActivate(bool active)
        {
            m_active = active;
        }
        bool isActive() const
        {
            return m_active;
        }
        float getCooldown()
        {
            return m_cooldown;
        }
        void setCooldown(float cooldown)
        {
            m_cooldown = cooldown;
        }

        virtual void activate(Player& player, QuadTree& quadtree) = 0;

        virtual void update(float deltaTime, Player& player, QuadTree& quadTree)
        {
            m_timeSinceLastFire += deltaTime;
            if(m_timeSinceLastFire >= m_cooldown)
            {
                activate(player, quadTree);
                m_timeSinceLastFire = 0.0f;
            }
        }
        virtual void render(sf::RenderWindow& window)
        {
            std::cout << window.getSize().x << std::endl;
        };
};

#endif
