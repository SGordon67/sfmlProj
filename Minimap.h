#ifndef MINIMAP_H
#define MINIMAP_H

#include "Player.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "UIElement.h"
#include <memory>

class Minimap : public UIElement
{
    private:
        std::shared_ptr<sf::View> m_view;
        sf::RectangleShape m_backgroundRect;
        sf::Color m_backgroundColor = sf::Color(128, 128, 128);
        sf::RectangleShape m_viewRect;
        sf::Color m_viewColor = sf::Color::White;
        float m_viewThicknessFraction = 80;
        sf::CircleShape m_playerShape;
        sf::Color m_playerColor = sf::Color::Red;
        float m_playerSizeFraction = 50;

        float minimapPositionXFraction = 0.755f;
        float minimapPositionYFraction = 0.01f;
        float minimapWidthFraction = 0.23f;
        float minimapHeightFraction = 0.23f;

    public:
        Minimap(std::shared_ptr<Player> player, std::shared_ptr<sf::View> view);
        std::shared_ptr<sf::View> getView() const;
        void updateViewport();
        void update(sf::RenderWindow& window, sf::Vector2f playerViewSize);
        void update(sf::RenderWindow& window) override;
        void render(sf::RenderWindow& window) override;
};

#endif
