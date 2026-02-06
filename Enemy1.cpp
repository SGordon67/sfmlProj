#include <iostream>

#include "Player.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "Enemy1.h"

        static constexpr sf::Vector2i d_enemy1Size = {13, 11};
        static constexpr int d_enemy1Mass = 10;
        static constexpr float d_enemy1Radius = d_enemy1Size.x/2.f;
        static constexpr sf::Vector2f d_enemy1Velocity = {0, 0};
        static constexpr float d_enemy1Acceleration = 5000;
        static constexpr float d_enemy1AngularVelocity = 0;
        static constexpr float d_enemy1MaxVelocity = 400;
        static constexpr float d_enemy1Drag = 4;

        static constexpr int d_enemy1HP = 100;
        static constexpr int d_enemy1MaxHP = 100;
        static constexpr int d_enemy1Damage = 10;

Enemy1::Enemy1(std::shared_ptr<Player> player)
    :Enemy(sf::Vector2f(0, 0), d_enemy1Size, M_PI/2, RenderLayer::Main, &enemyTexture,
            d_enemy1Mass, d_enemy1Radius, d_enemy1Velocity, d_enemy1Acceleration, d_enemy1AngularVelocity, d_enemy1MaxVelocity, d_enemy1Drag,
            d_enemy1HP, d_enemy1MaxHP, d_enemy1Damage, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, std::shared_ptr<Player> player)
    :Enemy(position, d_enemy1Size, M_PI/2, RenderLayer::Main, &enemyTexture,
            d_enemy1Mass, d_enemy1Radius, d_enemy1Velocity, d_enemy1Acceleration, d_enemy1AngularVelocity, d_enemy1MaxVelocity, d_enemy1Drag,
            d_enemy1HP, d_enemy1MaxHP, d_enemy1Damage, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
        int hp, int maxHP, int damage, std::shared_ptr<Player> player)
    :Enemy(position, size,rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag,
            hp, maxHP, damage, player)
{
}
Enemy1::Enemy1(const Enemy1& other)
    : Enemy(other)
{
}
Enemy1::Enemy1(Enemy1&& other) noexcept
: Enemy(std::move(other))
{
}
