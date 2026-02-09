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
        sf::CircleShape m_playerShape;
        sf::Color m_color;

        float minimapPositionXFraction = 0.755f;
        float minimapPositionYFraction = 0.01f;
        float minimapWidthFraction = 0.23f;
        float minimapHeightFraction = 0.23f;

    public:
        Minimap(std::shared_ptr<Player> player, std::shared_ptr<sf::View> view);
        std::shared_ptr<sf::View> getView() const;
        void updateViewport();
        void update(sf::RenderWindow& window) override;
        void render(sf::RenderWindow& window) override;
};

#endif
