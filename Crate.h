#ifndef CRATE_H
#define CRATE_H

#include "BasicObject.h"
#include "PhysicalObject.h"
#include "Interactable.h"
#include <memory>

class Crate : public PhysicalObject, public Interactable
{
    protected:
        // defaults
        static constexpr sf::Vector2i d_crateSize = {17, 17};
        static constexpr int d_crateMass = 10;
        static constexpr float d_crateRadius = 8;
        static constexpr sf::Vector2f d_crateVelocity = {0, 0};
        static constexpr float d_crateAcceleration = 0;
        static constexpr float d_crateAngularVelocity = 0;
        static constexpr float d_crateMaxVelocity = 500;
        static constexpr float d_crateDrag = 4;

        static constexpr float d_crateInteractRadius = 50;

        float m_interactRadius;
        bool m_used;
    public:
        Crate(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
                float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
                float interactRadius, bool used)
            : PhysicalObject(position, size, rotation, renderLayer, texture, mass, radius, velocity, acceleration, rotationVelocity, maxVelocity, drag)
              , m_interactRadius(interactRadius)
              , m_used(used)
    {
    }
        Crate(const Crate& other) // copy constructor
            : PhysicalObject(other)
              , m_interactRadius(other.m_interactRadius)
              , m_used(other.m_used)
    {
    }
        Crate(Crate&& other) noexcept // copy constructor
            : PhysicalObject(std::move(other))
            , m_interactRadius(other.m_interactRadius)
            , m_used(other.m_used)
            {
            }

        float getInteractionRadius() const override
        {
            return m_interactRadius;
        }
        void setInteractionRadius(float radius)
        {
            m_interactRadius = radius;
        }
        bool getUsed()
        {
            return m_used;
        }
        void setUsed(bool used)
        {
            m_used = used;
        }


        // interactable interface
        virtual int getObjectID() const override
        {
            return BasicObject::getObjectID();
        }
        virtual sf::Vector2f getPosition() const override
        {
            return BasicObject::getPosition();
        }
        virtual bool canInteract() const override
        {
            return true;
        }
        virtual void interact(std::shared_ptr<Player> player) override
        {
            std::cout << "INTERACTION (H) -- " << player->getObjectID() << " | " << getObjectID() << std::endl;
        }
};

#endif
