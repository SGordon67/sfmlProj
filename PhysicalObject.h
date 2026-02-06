#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H


#include "BasicObject.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

extern float degreesToRadians(float degrees);
extern void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime);
extern void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime);

class PhysicalObject : public BasicObject
{
    protected:
        float m_mass;
        float m_radius; // for collisions

        sf::Vector2f m_velocity;
        float m_acceleration;

        float m_angularVelocity;

        float m_maxVelocity;

        float m_dragCoef;

    public:
        PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
                float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity, float drag)
            : BasicObject(position, size, rotation, renderLayer, texture)
              , m_mass(mass)
              , m_radius(radius)
              , m_velocity(velocity)
              , m_acceleration(acceleration)
              , m_angularVelocity(angularVelocity)
              , m_maxVelocity(maxVelocity)
              , m_dragCoef(drag)
    {
    }
        PhysicalObject(const PhysicalObject& other) // copy constructor
            : BasicObject(other)
              , m_mass(other.m_mass)
              , m_radius(other.m_radius)
              , m_velocity(other.m_velocity)
              , m_acceleration(other.m_acceleration)
              , m_angularVelocity(other.m_angularVelocity)
              , m_maxVelocity(other.m_maxVelocity)
              , m_dragCoef(other.m_dragCoef)
    {
    }
        PhysicalObject(PhysicalObject&& other) noexcept // move constructor
            : BasicObject(std::move(other))
            , m_mass(other.m_mass)
            , m_radius(other.m_radius)
            , m_velocity(other.m_velocity)
            , m_acceleration(other.m_acceleration)
            , m_angularVelocity(other.m_angularVelocity)
            , m_maxVelocity(other.m_maxVelocity)
            , m_dragCoef(other.m_dragCoef)
            {
            }

        float getMass() const
        {
            return m_mass;
        }
        void setMass(float mass)
        {
            m_mass = mass;
        }

        float getRadius() const
        {
            return m_radius;
        }
        void setRadius(float radius)
        {
            m_radius = radius;
        }

        sf::Vector2f getVelocity() const
        {
            return m_velocity;
        }
        void setVelocity(sf::Vector2f velocity)
        {
            m_velocity = velocity;
        }

        float getAcceleration() const
        {
            return m_acceleration;
        }
        void setAcceleration(float acceleration)
        {
            m_acceleration = acceleration;
        }

        float getMaxVelocity() const
        {
            return m_maxVelocity;
        }
        void setMaxVelocity(float maxVelocity)
        {
            m_maxVelocity = maxVelocity;
        }

        float getAngularVelocity() const
        {
            return m_angularVelocity;
        }
        void setAngularVelocity(float angularVelocity)
        {
            m_angularVelocity = angularVelocity;
        }

        float getDragCoef() const
        {
            return m_dragCoef;
        }
        void setDragCoef(float drag)
        {
            m_dragCoef = drag;
        }

        void rotate(const float rotation)
        {
            m_sprite.rotate(sf::degrees(radiansToDegrees(rotation)));
            setRotation(getRotation() + rotation);

            // wrap the rotation
            if(getRotation() >= 2 * M_PI) setRotation(getRotation() - 2 * M_PI);
            if(getRotation() < 0) setRotation(getRotation() + 2 * M_PI);
        }
        virtual void updateRotation()
        {
            if(getAngularVelocity() == 0) return;
            rotate(getAngularVelocity() * FixedDeltaTime);
            setAngularVelocity(getAngularVelocity() * angularDrag);

            // clamp small values
            if(std::abs(getAngularVelocity()) < 0.01f)
            {
                setAngularVelocity(0.0f);
            }
        }

        virtual void updateVelocity(float accelerate, bool backward)
        {
            // zero out the velocity if its small enough
            if(getVelocity().x < 0.5 && getVelocity().x > -0.5) setVelocity({0, getVelocity().y});
            if(getVelocity().y < 0.5 && getVelocity().y > -0.5) setVelocity({getVelocity().x, 0});

            float direction = getRotation();
            if(backward) direction += M_PI;

            // manipulating the velocity directly, maybe bad practice?
            addDragForce(m_velocity, getMass(), FixedDeltaTime);
            addAccelerationForce(m_velocity, accelerate, direction, backward, getMaxVelocity(), getMass(), FixedDeltaTime);
        }

        virtual void updatePosition(float deltaTime)
        {
            setPosition(getPosition() + (getVelocity() * deltaTime));
        }

        virtual void physicalDraw(sf::RenderWindow& window)
        {
            sf::View wView = window.getView();
            sf::Vector2f viewCenter = wView.getCenter();
            sf::Vector2f viewSize = wView.getSize();

            auto isOnScreen = [&](const sf::Vector2f& pos)
            {
                float dx = std::abs(pos.x - viewCenter.x);
                float dy = std::abs(pos.y - viewCenter.y);
                return (dx < viewSize.x * 0.6f && dy < viewSize.y * 0.6f);
            };

            sf::Vector2f myClosestPosition = getClosestWrapPosition(viewCenter, getPosition());
            if(isOnScreen(myClosestPosition))
            {
                sf::Vector2f originalPosition = m_position;
                setPosition(myClosestPosition);
                window.draw(m_sprite);
                setPosition(originalPosition);

                if(showHitboxes)
                {
                    sf::CircleShape circle(getRadius());
                    circle.setOrigin(sf::Vector2f(getRadius(), getRadius()));
                    circle.setFillColor(hitboxColor);
                    circle.setPosition(myClosestPosition);
                    window.draw(circle);
                }
            }
        }

        virtual void physicalUpdate()
        {
            float accel = 0;
            bool backward = false;
            updateRotation();
            updateVelocity(accel, backward);
            updatePosition(FixedDeltaTime);
        }
};

#endif
