#ifndef ENEMY1_H
#define ENEMY1_H

#include "SFML/System/Vector2.hpp"

#include "Player.h"
#include "Enemy.h"

class Enemy1 : public Enemy
{
    private:
        // defaults
        static constexpr sf::Vector2i d_enemy1Size = {13, 11};
        static constexpr int d_enemy1Mass = 10;
        static constexpr float d_enemy1Radius = d_enemy1Size.x/2.f;
        static constexpr sf::Vector2f d_enemy1Velocity = {0, 0};
        static constexpr float d_enemy1Acceleration = 5000;
        static constexpr float d_enemy1AngularVelocity = 0;
        static constexpr float d_enemy1MaxVelocity = 450;
        static constexpr float d_enemy1Drag = 5;

        static constexpr int d_enemy1HP = 100;
        static constexpr int d_enemy1MaxHP = 100;
        static constexpr int d_enemy1Damage = 10;
    public:
        Enemy1(std::shared_ptr<Player> player);
        Enemy1(sf::Vector2f position, std::shared_ptr<Player> player);
        Enemy1(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
                float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
                int hp, int maxHP, int damage, std::shared_ptr<Player> player);
        Enemy1(const Enemy1& other); // copy constructor
        Enemy1(Enemy1&& other) noexcept; // move constructor

        virtual void update() override
        {
            float accel = getAcceleration();
            bool backward = false;
            updateRotation();
            updateVelocity(accel, backward);
            updatePosition(FixedDeltaTime);
        }
};

#endif
