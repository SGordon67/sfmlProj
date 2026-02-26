#include "Minimap.h"
#include "SFML/System/Vector2.hpp"
#include "UIElement.h"
#include "globals.h"

Minimap::Minimap(std::shared_ptr<Player> player, std::shared_ptr<sf::View> view)
    : UIElement(player)
    , m_view(view)
    , m_playerShape(sf::CircleShape((((m_view->getSize().y + m_view->getSize().x) / 2) / 50)))
{
    m_playerShape.setFillColor(m_playerColor);
    m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
    m_playerShape.setPosition(m_player->getPosition());

    m_backgroundRect.setSize(sf::Vector2f(worldWidth, worldHeight));
    m_backgroundRect.setFillColor(m_backgroundColor);
    m_backgroundRect.setPosition(sf::Vector2f(0, 0));

    // m_viewRect.setSize(sf::Vector2f(view->getSize().x, view->getSize().y));
    m_viewRect.setSize({10, 10});
    m_viewRect.setOrigin({m_viewRect.getSize().x / 2.f, m_viewRect.getSize().y / 2.f});
    m_viewRect.setPosition(player->getPosition());
    m_viewRect.setOutlineColor(m_viewColor);
    m_viewRect.setOutlineThickness(((view->getSize().y + view->getSize().x) / 2) / m_viewThicknessFraction);
    m_viewRect.setFillColor(sf::Color::Transparent);
}

std::shared_ptr<sf::View> Minimap::getView() const
{
    return m_view;
}

void Minimap::updateViewport()
{
    // size the minimap based on window height, size width so it's square proportion
    minimapHeightFraction = 0.23f;
    minimapWidthFraction = ((0.23f * viewHeight) / viewWidth);

    minimapPositionXFraction =  1 - minimapWidthFraction - 0.015;

    m_view->setViewport(sf::FloatRect({minimapPositionXFraction, minimapPositionYFraction}, {minimapWidthFraction, minimapHeightFraction}));
}

void Minimap::update(sf::RenderWindow& window, sf::Vector2f playerViewSize)
{
    // update player icon on the map
    m_playerShape = sf::CircleShape(((window.getView().getSize().y + window.getView().getSize().x) / 2) / m_playerSizeFraction);
    m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
    m_playerShape.setPosition(m_player->getPosition());


    m_viewRect.setPosition(m_player->getPosition());
    m_playerShape.setPosition(m_player->getPosition());


    // m_viewRect.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
    m_viewRect.setSize(playerViewSize);
    m_viewRect.setOrigin({m_viewRect.getSize().x / 2.f, m_viewRect.getSize().y / 2.f});
    m_viewRect.setPosition(m_player->getPosition());
}
void Minimap::update(sf::RenderWindow& window)
{
    // update player icon on the map
    m_playerShape = sf::CircleShape(((window.getView().getSize().y + window.getView().getSize().x) / 2) / 50);
    m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
    m_playerShape.setPosition(m_player->getPosition());


    m_viewRect.setPosition(m_player->getPosition());
    m_playerShape.setPosition(m_player->getPosition());


    // m_viewRect.setSize(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
    m_viewRect.setSize({10, 10});
    m_viewRect.setOrigin({m_viewRect.getSize().x / 2.f, m_viewRect.getSize().y / 2.f});
    m_viewRect.setPosition(m_player->getPosition());
}
void Minimap::render(sf::RenderWindow& window)
{
    window.draw(m_backgroundRect);
    window.draw(m_viewRect);
    sf::Vector2f originalPosition = m_viewRect.getPosition();
    std::vector<sf::Vector2f> viewRectPositions = getDupPositions(m_viewRect.getPosition(), windowWidth);
    for(auto pos : viewRectPositions)
    {
        m_viewRect.setPosition(pos);
        m_playerShape.setPosition(pos);
        window.draw(m_viewRect);
        window.draw(m_playerShape);
    }
    m_viewRect.setPosition(originalPosition);
    m_playerShape.setPosition(originalPosition);
    window.draw(m_playerShape);
}
