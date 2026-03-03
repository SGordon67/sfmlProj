#ifndef FIREBALLWEAPON_H
#define FIREBALLWEAPON_H

#include "Weapon.h"

class FireballWeapon : public Weapon
{
    protected:
        // from weapon
        // std::string m_name = "Weapon";
        // int m_damage = 10;
        // float m_cooldown = 3;
        // float m_size = 10;
        // sf::Vector2f m_velocity = {0, 0};
        // float m_duration = 1;
        // float m_kb = 0;
        // float m_timeSinceLastFire = m_cooldown;
        // bool m_active = false;

        sf::CircleShape m_visualEffect;
        float m_visualTimer = 0.f;
        float m_visualDuration = 0.1f;
    public:
        FireballWeapon();
        FireballWeapon(int damage, float cooldown, float radius);
        FireballWeapon(const FireballWeapon& other); // copy constructor
        FireballWeapon(FireballWeapon&& other) noexcept; // move constructor

        float getRadius();
        void setRadius(float radius);

        void activate(Player& player, QuadTree& quadTree) override;
        void update(float deltaTime, Player& player, QuadTree& quadTree) override;
        void render(sf::RenderWindow& window) override;
};

#endif

