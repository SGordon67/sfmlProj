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
    m_rect = sf::RectangleShape();
    m_rect.setSize(sf::Vector2f(windowWidth * barWidth, windowHeight * barHeight));
    m_rect.setPosition(sf::Vector2f(windowWidth * xMarginMultiplier, windowHeight * yMarginMultiplier));
    m_rect.setFillColor(bgColor);
}
UIHealth::UIHealth(const UIHealth& other) // copy constructor
	: UIElement(other)
      , m_rect(other.m_rect)
{
}
UIHealth::UIHealth(UIHealth&& other) noexcept // move constructor
	: UIElement(std::move(other))
      , m_rect(other.m_rect)
{
}

void UIHealth::update()
{
    // std::cout << "Updating healthbar..." << std::endl;
}
void UIHealth::render(sf::RenderWindow& window)
{
    float currentViewWidth = window.getView().getSize().x;
    float currentViewHeight = window.getView().getSize().y;

    // std::cout << "Rendering healthbar..." << std::endl;

    // left side full health bar
    m_rect.setFillColor(bgColor);
    window.draw(m_rect);

    sf::Vector2f originalPosition = m_rect.getPosition();
    sf::Vector2f originalSize = m_rect.getSize();

    // right side full health bar
    m_rect.setPosition(sf::Vector2f(currentViewWidth - (currentViewWidth * xMarginMultiplier) - m_rect.getSize().x, originalPosition.y));
    window.draw(m_rect);


    // get fraction of health
    float healthPercentage = (float)m_player->getHP() / (float)m_player->getMaxHP();
    
    if(healthPercentage > 0)
    {
        // right side fractional
        m_rect.setSize(sf::Vector2f(m_rect.getSize().x, m_rect.getSize().y * healthPercentage));
        m_rect.setPosition(sf::Vector2f(m_rect.getPosition().x, m_rect.getPosition().y + (originalSize.y * (1 - healthPercentage))));
        m_rect.setFillColor(hpColor);
        window.draw(m_rect);

        // left side fractional
        m_rect.setPosition(sf::Vector2f(originalPosition.x, m_rect.getPosition().y));
        window.draw(m_rect);
    }

    // reset the rectangle
    m_rect.setSize(sf::Vector2f(currentViewWidth * barWidth, currentViewHeight * barHeight));
    m_rect.setPosition(sf::Vector2f(currentViewWidth * xMarginMultiplier, currentViewHeight * yMarginMultiplier));
}
