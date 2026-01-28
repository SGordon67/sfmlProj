#ifndef WEAPON_H
#define WEAPON_H

#include <iostream>

class Player;

class Weapon
{
    protected:
        int m_damage;
        float m_cooldown;
        float m_timeSinceLastFire;
        bool m_active;

    public:
        Weapon()
            : m_damage(10)
              , m_cooldown(1)
              , m_timeSinceLastFire(m_cooldown)
              , m_active(false)
        {
        }
        Weapon(const Weapon& other) // copy constructor
            : m_damage(other.m_damage)
              , m_cooldown(other.m_cooldown)
              , m_timeSinceLastFire(other.m_timeSinceLastFire)
              , m_active(other.m_active)
        {
        }
        Weapon(Weapon&& other) noexcept // move constructor
            : m_damage(other.m_damage)
              , m_cooldown(other.m_cooldown)
              , m_timeSinceLastFire(other.m_timeSinceLastFire)
              , m_active(other.m_active)
        {
        }

        virtual void update(float deltaTime)
        {
            std::cout << "Hello world: " << deltaTime << std::endl;
        }
};

#endif
