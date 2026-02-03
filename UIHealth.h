#ifndef UIHEALTH_H
#define UIHEALTH_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "UIElement.h"
#include <memory>

class UIHealth : public UIElement
{
    protected:
        sf::RectangleShape m_fullRect;
        sf::RectangleShape m_partRect;
    public:
        UIHealth(std::shared_ptr<Player> player);
        UIHealth(const UIHealth& other); // copy constructor
        UIHealth(UIHealth&& other) noexcept; // move constructor
        sf::RectangleShape getFullRect()
        {
            return m_fullRect;
        }
        void setFullRect(sf::RectangleShape shape)
        {
            m_fullRect = shape;
        }
        sf::RectangleShape getPartRect()
        {
            return m_partRect;
        }
        void setPartRect(sf::RectangleShape shape)
        {
            m_partRect = shape;
        }

        virtual void update(sf::RenderWindow& window) override;
        virtual void render(sf::RenderWindow& window) override;
};

#endif
