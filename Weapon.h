#ifndef WEAPON_H
#define WEAPON_H

#include "QuadTree.h"
#include <iostream>

class Player;

class Weapon
{
    protected:
        std::string m_name = "Weapon";

        int m_damage = 10;
        float m_cooldown = 3;
        float m_size = 10;
        sf::Vector2f m_velocity = {0, 0};
        float m_duration = 1;
        float m_kb = 0;

        float m_timeSinceLastFire = m_cooldown;
        bool m_active = false;

    public:
        Weapon()
        {
        }
        Weapon(int damage, float cooldown, float size, sf::Vector2f velocity, float duration, float kb
                , float timeSince, bool active)
            : m_damage(damage)
              , m_cooldown(cooldown)
              , m_size(size)
              , m_velocity(velocity)
              , m_duration(duration)
              , m_kb(kb)
              , m_timeSinceLastFire(timeSince)
              , m_active(active)
    {
    }
        Weapon(const Weapon& other) // copy constructor
            : m_damage(other.m_damage)
              , m_cooldown(other.m_cooldown)
              , m_size(other.m_size)
              , m_velocity(other.m_velocity)
              , m_duration(other.m_duration)
              , m_kb(other.m_kb)
              , m_timeSinceLastFire(other.m_timeSinceLastFire)
              , m_active(other.m_active)
    {
    }
        Weapon(Weapon&& other) noexcept // move constructor
            : m_damage(other.m_damage)
            , m_cooldown(other.m_cooldown)
            , m_size(other.m_size)
            , m_velocity(std::move(other.m_velocity))
            , m_duration(other.m_duration)
            , m_kb(other.m_kb)
            , m_timeSinceLastFire(other.m_timeSinceLastFire)
            , m_active(other.m_active)
            {
            }

        virtual ~Weapon() = default;

        std::string getName() { return m_name; }
        bool getActive() const { return m_active; }
        float getCooldown() { return m_cooldown; }
        float getSize() { return m_size; }
        sf::Vector2f getVelocity() { return m_velocity; }
        float getDuration() { return m_duration; }
        float getKB() { return m_kb; }

        void setName(std::string name) { m_name = name; }
        void setActive(bool active) { m_active = active; }
        void setCooldown(float cooldown) { m_cooldown = cooldown; }
        void setSize(float size) { m_size = size; }
        void setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }
        void setDuration(float duration) { m_duration = duration; }
        void setKB(float kb) { m_kb = kb; }
        void setActivate(bool active) { m_active = active; }

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
