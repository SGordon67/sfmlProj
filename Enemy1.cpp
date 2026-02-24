#include <iostream>

#include "Player.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "Enemy1.h"

Enemy1::Enemy1(std::shared_ptr<Player> player, std::vector<std::unique_ptr<VisualObject>>* visualObjects)
    :Enemy(sf::Vector2f(0, 0), d_enemy1Size, M_PI/2, RenderLayer::Main, &enemyTexture,
            d_enemy1Mass, d_enemy1Radius, d_enemy1Velocity, d_enemy1Acceleration, d_enemy1AngularVelocity, d_enemy1MaxVelocity, d_enemy1Drag,
            d_enemy1HP, d_enemy1MaxHP, visualObjects, d_enemy1Damage, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, std::shared_ptr<Player> player, std::vector<std::unique_ptr<VisualObject>>* visualObjects)
    :Enemy(position, d_enemy1Size, M_PI/2, RenderLayer::Main, &enemyTexture,
            d_enemy1Mass, d_enemy1Radius, d_enemy1Velocity, d_enemy1Acceleration, d_enemy1AngularVelocity, d_enemy1MaxVelocity, d_enemy1Drag,
            d_enemy1HP, d_enemy1MaxHP, visualObjects, 
            d_enemy1Damage, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
        int hp, int maxHP, std::vector<std::unique_ptr<VisualObject>>* visualObjects, 
        int damage, std::shared_ptr<Player> player)
    :Enemy(position, size,rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag,
            hp, maxHP, visualObjects, 
            damage, player)
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
