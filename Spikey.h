#ifndef SPIKEY_H
#define SPIKEY_H

#include "SFML/System/Vector2.hpp"

#include "Entity.h"
#include "Hazardous.h"

class Spikey : public Entity, public Hazardous
{
    private:
        // defaults
        static constexpr sf::Vector2i d_spikeySize = {20, 20};
        static constexpr int d_spikeyMass = 10;
        static constexpr float d_spikeyRadius = 8;
        static constexpr sf::Vector2f d_spikeyVelocity = {0, 0};
        static constexpr float d_spikeyAcceleration = 0;
        static constexpr float d_spikeyAngularVelocity = 0;
        static constexpr float d_spikeyMaxVelocity = 500;
        static constexpr float d_spikeyDrag = 4;

        static constexpr int d_spikeyHP = 100;
        static constexpr int d_spikeyMaxHP = 100;

        int m_damage;
    public:
        Spikey(std::vector<std::unique_ptr<VisualObject>>* m_visualObjects);
        Spikey(sf::Vector2f position, std::vector<std::unique_ptr<VisualObject>>* m_visualObjects);
        Spikey(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
                float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
                int hp, int maxHP, std::vector<std::unique_ptr<VisualObject>>* m_visualObjects, int damage);
        Spikey(const Spikey& other); // copy constructor
        Spikey(Spikey&& other) noexcept; // move constructor

        int getDamage() const;
        void setDamage(int damage);

        // interface hazardous
        virtual int getObjectID() const override;
        virtual sf::Vector2f getPosition() const override;
        virtual float getRadius() const override;
        virtual void dealDamage(std::shared_ptr<Entity> entity) override;
        virtual void update() override;
};

#endif
