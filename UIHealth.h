#ifndef UIHEALTH_H
#define UIHEALTH_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "UIElement.h"
#include <memory>

class UIHealth : public UIElement
{
    protected:
        sf::RectangleShape m_rect;
    public:
        UIHealth(std::shared_ptr<Player> player);
        UIHealth(const UIHealth& other); // copy constructor
        UIHealth(UIHealth&& other) noexcept; // move constructor
        sf::RectangleShape getRect()
        {
            return m_rect;
        }
        void setRect(sf::RectangleShape shape)
        {
            m_rect = shape;
        }

        virtual void update() override;
        virtual void render(sf::RenderWindow& window) override;
};

#endif
