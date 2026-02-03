#include "Minimap.h"
#include "SFML/System/Vector2.hpp"
#include "UIElement.h"
#include "globals.h"

Minimap::Minimap(std::shared_ptr<Player> player, std::shared_ptr<sf::View> view)
    : UIElement(player)
    , m_view(view)
    , m_playerShape(sf::CircleShape((((m_view->getSize().y + m_view->getSize().x) / 2) / 50)))
      , m_color(sf::Color::Red)
{
    m_playerShape.setFillColor(m_color);
    m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
    m_playerShape.setPosition(m_player->getPosition());

    m_backgroundRect.setSize(sf::Vector2f(worldWidth, worldHeight));
    m_backgroundRect.setFillColor(sf::Color(128, 128, 128));
    m_backgroundRect.setPosition(sf::Vector2f(0, 0));
}

std::shared_ptr<sf::View> Minimap::getView() const
{
    return m_view;
}

void Minimap::update(sf::RenderWindow& window)
{
    float currentViewWidth = window.getView().getSize().x;
    float currentViewHeight = window.getView().getSize().y;

    std::cout << "View size: " << currentViewWidth << ", " << currentViewHeight << std::endl;
    std::cout << "Window size: " << windowWidth << ", " << windowHeight << std::endl;

    // update player icon on the map
    m_playerShape = sf::CircleShape(((window.getView().getSize().y + window.getView().getSize().x) / 2) / 50);
    m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
    m_playerShape.setPosition(m_player->getPosition());
}
void Minimap::render(sf::RenderWindow& window)
{
    window.draw(m_backgroundRect);
    window.draw(m_playerShape);
}
