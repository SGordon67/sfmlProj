#include "UIKillCount.h"

#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include "UIKillCount.h"
#include "Player.h"
#include <memory>

UIKillCount::UIKillCount(std::shared_ptr<Player> player)
    : UIElement(player)
      , m_killCount(&killCount)
      , m_text(sf::Text(font))
{
    m_text.setString(std::to_string(getKillCount()));
    m_text.setFillColor(textColor);
}
UIKillCount::UIKillCount(const UIKillCount& other) // copy constructor
	: UIElement(other)
      , m_killCount(&killCount)
      , m_text(sf::Text(font))
{
    m_text.setString(std::to_string(getKillCount()));
    m_text.setFillColor(textColor);
}
UIKillCount::UIKillCount(UIKillCount&& other) noexcept // move constructor
	: UIElement(std::move(other))
      , m_killCount(&killCount)
      , m_text(sf::Text(font))
{
    m_text.setString(std::to_string(getKillCount()));
    m_text.setFillColor(textColor);
}

int UIKillCount::getKillCount() const
{
    return *m_killCount;
}
void UIKillCount::setKillCount(int count)
{
    *m_killCount = count;
}

void UIKillCount::update(sf::RenderWindow& window)
{
    float currentViewWidth = window.getView().getSize().x;
    float currentViewHeight = window.getView().getSize().y;

    m_text.setString(std::to_string(getKillCount()));

    m_text.setCharacterSize(currentViewHeight * textHeightFraction);
    m_text.setPosition(sf::Vector2f(currentViewWidth * textPositionXFraction, currentViewHeight * textPositionYFraction));
}

void UIKillCount::render(sf::RenderWindow& window)
{
    window.draw(m_text);
}
