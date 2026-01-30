#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "UIHealth.h"
#include "Player.h"
#include <memory>

float barWidth = 0.005;
float barHeight = 0.98;
float xMarginMultiplier = 0.005;
float yMarginMultiplier = (1.0 - barHeight) / 2; // centered vertically

sf::Color bgColor = sf::Color::Cyan;
sf::Color hpColor = sf::Color::Red;

UIHealth::UIHealth(std::shared_ptr<Player> player)
	: UIElement(player)
{
    m_fullRect = sf::RectangleShape();
    m_fullRect.setSize(sf::Vector2f(windowWidth * barWidth, windowHeight * barHeight));
    m_fullRect.setPosition(sf::Vector2f(windowWidth * xMarginMultiplier, windowHeight * yMarginMultiplier));
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

void UIHealth::update()
{
    // get fraction of health
    float healthPercentage = (float)m_player->getHP() / (float)m_player->getMaxHP();

    m_partRect.setSize(sf::Vector2f(m_fullRect.getSize().x, m_fullRect.getSize().y * healthPercentage));
    m_partRect.setPosition(sf::Vector2f(m_fullRect.getPosition().x, m_fullRect.getPosition().y + (m_fullRect.getSize().y * (1 - healthPercentage))));
}

void UIHealth::render(sf::RenderWindow& window)
{
    float currentViewWidth = window.getView().getSize().x;

    // left side full health bar
    window.draw(m_fullRect);

    sf::Vector2f originalPosition = m_fullRect.getPosition();

    // right side full health bar
    m_fullRect.setPosition(sf::Vector2f(currentViewWidth - (currentViewWidth * xMarginMultiplier) - m_fullRect.getSize().x, originalPosition.y));
    window.draw(m_fullRect);

    // reset to left side
    m_fullRect.setPosition(originalPosition);


    // get fraction of health
    float healthPercentage = (float)m_player->getHP() / (float)m_player->getMaxHP();

    if(healthPercentage > 0)
    {
        // left side partial health
        window.draw(m_partRect);

        // right side partial health
        m_partRect.setPosition(sf::Vector2f(currentViewWidth - (currentViewWidth * xMarginMultiplier) - m_fullRect.getSize().x, m_partRect.getPosition().y));
        window.draw(m_partRect);

        // reset to left side
        m_partRect.setPosition(sf::Vector2f(m_fullRect.getPosition().x, m_partRect.getPosition().y));
    }
}
