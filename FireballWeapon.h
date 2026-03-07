#ifndef FIREBALLWEAPON_H
#define FIREBALLWEAPON_H

#include "Fireball.h"
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

        std::vector<std::unique_ptr<Fireball>> m_activeFireballs;
        float m_speed = 200;
        float m_range = 300;
    public:
        FireballWeapon();
        FireballWeapon(int damage, float cooldown, float speed, float radius);

        void activate(Player& player, QuadTree& quadTree) override;
        void update(float deltaTime, Player& player, QuadTree& quadTree) override;
        void render(sf::RenderWindow& window) override;
};

#endif

