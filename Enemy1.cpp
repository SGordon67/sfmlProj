#include <iostream>

#include "Player.h"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "Enemy1.h"

	sf::Vector2f ePosition({100, 190});
	sf::Vector2i eSize(13, 11);
	float eRotation = M_PI / 2;
	RenderLayer eRenderLayer = RenderLayer::Main;
	std::string eFilename = "art/basicEnemy.png";
	float eMass = 10;
	float eRadius = eSize.x / 2.f;
	sf::Vector2f eVelocity = {0, 0};
	float eAcceleration = 5000;
	float eAngularVelocity = 0;
	float eMaximumVelocity = 400; // players is 500 currently
    int eHP = 100;
    int eMHP = 100;
    int eDamage = 10;



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
