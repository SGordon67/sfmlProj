#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "UIHealth.h"
#include "Player.h"
#include <memory>

UIHealth::UIHealth(std::shared_ptr<Player> player)
	: UIElement(player)
{
    m_fullRect = sf::RectangleShape();
    m_fullRect.setSize(sf::Vector2f(windowWidth * barWidthFraction, windowHeight * barHeightFraction));
    m_fullRect.setPosition(sf::Vector2f(windowWidth * xMarginFraction, windowHeight * yMarginFraction));
    m_fullRect.setFillColor(bgColor);
    m_partRect = m_fullRect;
    
    m_fullRect.setFillColor(bgColor);
    m_partRect.setFillColor(hpColor);
}
UIHealth::UIHealth(const UIHealth& other) // copy constructor
	: UIElement(other)
      , m_fullRect(other.m_fullRect)
      , m_partRect(other.m_partRect)
{
}
UIHealth::UIHealth(UIHealth&& other) noexcept // move constructor
	: UIElement(std::move(other))
      , m_fullRect(other.m_fullRect)
      , m_partRect(other.m_partRect)
{
}

void UIHealth::update(sf::RenderWindow& window)
{
    float currentViewWidth = window.getView().getSize().x;
    float currentViewHeight = window.getView().getSize().y;

    // std::cout << "View size: " << currentViewWidth << ", " << currentViewHeight << std::endl;
    // std::cout << "Window size: " << windowWidth << ", " << windowHeight << std::endl;

    // in case of window resize
    m_fullRect.setSize(sf::Vector2f(currentViewWidth * barWidthFraction, currentViewHeight * barHeightFraction));
    m_fullRect.setPosition(sf::Vector2f(currentViewWidth * xMarginFraction, currentViewHeight * yMarginFraction));


    // get fraction of health
    float healthPercentage = (float)m_player->getHP() / (float)m_player->getMaxHP();
    m_partRect.setSize(sf::Vector2f(m_fullRect.getSize().x, m_fullRect.getSize().y * healthPercentage));
    m_partRect.setPosition(sf::Vector2f(m_fullRect.getPosition().x, m_fullRect.getPosition().y + (m_fullRect.getSize().y * (1 - healthPercentage))));
}

void UIHealth::render(sf::RenderWindow& window)
{
// void Minimap::update(sf::RenderWindow& window)
// {
//     // update player icon on the map
//     m_playerShape = sf::CircleShape(((window.getView().getSize().y + window.getView().getSize().x) / 2) / 50);
//     m_playerShape.setOrigin({m_playerShape.getRadius(), m_playerShape.getRadius()});
//     m_playerShape.setPosition(m_player->getPosition());
// }
    // height not needed, just need width to render on both sides of the screen
    float currentViewWidth = window.getView().getSize().x;

    // left side full health bar
    window.draw(m_fullRect);

    // right side full health bar
    sf::Vector2f originalPosition = m_fullRect.getPosition();
    m_fullRect.setPosition(sf::Vector2f(currentViewWidth - (currentViewWidth * xMarginFraction) - m_fullRect.getSize().x, originalPosition.y));
    window.draw(m_fullRect);
    m_fullRect.setPosition(originalPosition);


    // get fraction of health
    float healthPercentage = (float)m_player->getHP() / (float)m_player->getMaxHP();
    if(healthPercentage > 0)
    {
        // left side partial health
        window.draw(m_partRect);

        // right side partial health
        m_partRect.setPosition(sf::Vector2f(currentViewWidth - (currentViewWidth * xMarginFraction) - m_fullRect.getSize().x, m_partRect.getPosition().y));
        window.draw(m_partRect);

        // reset to left side
        m_partRect.setPosition(sf::Vector2f(m_fullRect.getPosition().x, m_partRect.getPosition().y));
    }
}
