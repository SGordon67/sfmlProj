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

        static constexpr int d_damage = 10;
        static constexpr float d_cooldown = 0.1;
        static constexpr float d_size = 20;
        static constexpr float d_duration = 2;
        static constexpr float d_kb = 0;
        static constexpr float d_range = 900;
        static constexpr float d_fbRange = 500;

        static constexpr sf::Vector2i d_fireballWeaponSize = {20, 20};
        static constexpr int d_fireballWeaponMass = 0;
        static constexpr float d_fireballWeaponRadius = 10;
        static constexpr float d_fireballWeaponMaxVelocity = 200;

        sf::Vector2f m_position = {0, 0};
        std::vector<std::unique_ptr<Fireball>> m_activeFireballs;
        float m_range; // if enemy within range, fire a fireball
        float m_fbRange; // range fireball will travel
    public:
        FireballWeapon();
        FireballWeapon(int damage, float cooldown, float speed, float radius);

        float getRange();
        sf::Vector2f getPosition();

        void setRange(float range);
        void setPosition(sf::Vector2f pos);

        void activate(Player& player, QuadTree& quadTree) override;
        void update(float deltaTime, Player& player, QuadTree& quadTree) override;
        void render(sf::RenderWindow& window) override;
};

#endif

