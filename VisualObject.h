#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "BasicObject.h"

class VisualObject : public BasicObject
{
    protected:
        bool m_markedForDelete = false;

    public:
        VisualObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture)
            : BasicObject(position, size, rotation, renderLayer, texture)
        {
        }
        VisualObject(const VisualObject& other)
            : BasicObject(other)
        {
        }
        VisualObject(VisualObject&& other) noexcept
            : BasicObject(std::move(other))
            {
            }

        bool getMarkedForDelete()
        {
            return m_markedForDelete;
        }
        void markForDelete()
        {
            m_markedForDelete = true;
        }

        virtual void visualUpdate(float deltaTime, sf::Vector2f relationalVelocity)
        {
            basicUpdate(deltaTime, relationalVelocity);
        }
        virtual void draw(sf::RenderWindow& window)
        {
            VisualObject::basicDraw(window);
        }
};

#endif
