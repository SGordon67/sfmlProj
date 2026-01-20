#ifndef ENEMY_H
#define ENEMY_H

#include "SFML/System/Vector2.hpp"

#include "Player.h"
#include "Entity.h"
#include "Hazardous.h"

class Enemy : public Entity, public Hazardous
{
private:
	int m_damage;
    Player* m_player;
public:
    Enemy(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, std::string filename, 
            float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, 
            int hp, int maxHP, int damage, Player& player)
        : Entity(position, size, rotation, renderLayer, filename, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, hp, maxHP)
          , m_damage(damage)
          , m_player(&player)
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

        float angle = std::atan2(pPosition.y - m_position.y, pPosition.x - m_position.x);
        setRotation(angle);
        m_sprite.setRotation(sf::degrees(radiansToDegrees(angle)));
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
	virtual void dealDamage(Entity& entity) override
    {
        entity.reduceHealth(getDamage());
    }
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
