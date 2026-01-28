#include "Minimap.h"
#include "SFML/System/Vector2.hpp"
#include "globals.h"

Minimap::Minimap(std::shared_ptr<Player> player, std::shared_ptr<sf::View> view)
    : m_player(player)
    , m_view(view)
    , m_playerShape(sf::CircleShape(view->getSize().y / 50))
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

void Minimap::update()
{
}
void Minimap::draw(sf::RenderWindow& window)
{
    m_playerShape.setPosition(m_player->getPosition());
    window.setView(*getView());
    window.draw(m_backgroundRect);
    window.draw(m_playerShape);
}
