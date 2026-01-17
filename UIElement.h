#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <iostream>

#include "Player.h"

class UIElement
{
    protected:
        Player* m_player;

    public:
        UIElement(Player* player)
            : m_player(player)
        {
            std::cout << "Regular constructor called for basic object" << std::endl;
        }
        UIElement(const UIElement& other) // copy constructor
            : m_player(other.m_player)
        {
            std::cout << "Copy constructor called for basic object" << std::endl;
        }
        UIElement(UIElement&& other) noexcept // move constructor
            : m_player(std::move(other.m_player))
            {
                std::cout << "Move constructor called for basic object" << std::endl;
            }

        virtual void update() = 0;
        virtual void render(sf::RenderWindow& window) = 0;
};

#endif
