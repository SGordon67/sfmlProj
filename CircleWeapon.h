#ifndef CIRCLEWEAPON_H
#define CIRCLEWEAPON_H

#include "Weapon.h"

class CircleWeapon : public Weapon
{
    protected:
        // from weapon
        // int m_damage;
        // float m_cooldown;
        // float m_timeSinceLastFire;
        // bool m_active;

        std::string m_name = "CircleWeapon";
        float m_radius;
        sf::CircleShape m_visualEffect;
        float m_visualTimer = 0.f;
        float m_visualDuration = 0.1f;
    public:
        CircleWeapon();
        CircleWeapon(int damage, float cooldown, float radius);
        CircleWeapon(const CircleWeapon& other); // copy constructor
        CircleWeapon(CircleWeapon&& other) noexcept; // move constructor

        void activate(Player& player, QuadTree& quadTree) override;
        void update(float deltaTime, Player& player, QuadTree& quadTree) override;
        void render(sf::RenderWindow& window) override;
};

#endif
