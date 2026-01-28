#include <iostream>

#include "Player.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "Enemy1.h"

Enemy1::Enemy1(std::shared_ptr<Player> player)
    :Enemy(sf::Vector2f(0, 0), sf::Vector2i(13, 11), M_PI/2, RenderLayer::Main, &enemyTexture, 
            10, 13/2.f, sf::Vector2f(0, 0), 5000, 0, 400,
            100, 100, 10, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, std::shared_ptr<Player> player)
    :Enemy(position, sf::Vector2i(13, 11), M_PI/2, RenderLayer::Main, &enemyTexture, 
            10, 13/2.f, sf::Vector2f(0, 0), 5000, 0, 400,
            100, 100, 10, player)
{
}
Enemy1::Enemy1(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
        int hp, int maxHP, int damage, std::shared_ptr<Player> player)
    :Enemy(position, size,rotation, renderLayer, texture, 
            mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, 
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
