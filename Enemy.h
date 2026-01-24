#ifndef ENEMY_H
#define ENEMY_H

#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Angle.hpp"
#include "SFML/System/Vector2.hpp"

#include "Player.h"
#include "Entity.h"
#include "Hazardous.h"
#include "globals.h"
#include <memory>

class Enemy : public Entity, public Hazardous
{
private:
	int m_damage;
    std::shared_ptr<Player> m_player;
public:
    Enemy(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
            int hp, int maxHP, int damage, std::shared_ptr<Player> player)
        : Entity(position, size, rotation, renderLayer, texture, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
          , m_damage(damage)
          , m_player(player)
    {
    }
    Enemy(const Enemy& other)
        : Entity(other)
          , m_damage(other.m_damage)
          , m_player(other.m_player)
    {
    }
    Enemy(Enemy&& other) noexcept
        : Entity(std::move(other))
        , m_damage(other.m_damage)
        , m_player(other.m_player)
        {
        }

    int getDamage()
    {
        return m_damage;
    }
    void setDamage(int damage)
    {
        m_damage = damage;
    }

    void updateRotation() override
    {
        // face toward the player
        sf::Vector2f pPosition = m_player->getPosition();
        sf::Vector2f delta = pPosition - m_position;

        // adjust for wrapping
        if(std::abs(delta.x) > worldWidth / 2.0f)
        {
            delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
        }
        if(std::abs(delta.y) > worldHeight / 2.0f)
        {
            delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
        }

        float angle = std::atan2(delta.y, delta.x);

        setRotation(angle + M_PI);
        m_sprite.setRotation(sf::radians(angle + M_PI/2));
    }

    virtual int getObjectID() const override
    {
        return BasicObject::getObjectID();
    }
	virtual sf::Vector2f getPosition() const override
    {
        return BasicObject::getPosition();
    }
	virtual float getRadius() const override
    {
        return PhysicalObject::getRadius();
    }
	virtual void dealDamage(std::shared_ptr<Entity> entity) override
    {
        entity->reduceHealth(getDamage());
    }
    virtual void physicalUpdate() override
    {
        float accel = getAcceleration();
        bool backward = false;
        updateRotation();
        updateVelocity(accel, backward);
        updatePosition(FixedDeltaTime);
    }
    virtual void update() override
    {
    }
};

#endif
