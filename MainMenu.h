#ifndef MAINMENU_H
#define MAINMENU_H

#include "globals.h"
#include <SFML/Graphics.hpp>

class MainMenu
{
    private:
        sf::Text m_title;
        sf::Text m_startButton;
        sf::RectangleShape m_startButtonBg;

    public:
        MainMenu()
            : m_title(sf::Text(font))
              , m_startButton(sf::Text(font))
        {
            m_title.setString("Meteorite");
            m_title.setCharacterSize(72);
            m_title.setFillColor(sf::Color::White);

            m_startButton.setString("Start Game");
            m_startButton.setCharacterSize(36);
            m_startButton.setFillColor(sf::Color::White);

            m_startButtonBg.setSize({250, 60});
            m_startButtonBg.setFillColor(sf::Color(50, 50, 150));
            m_startButtonBg.setOutlineThickness(3);
            m_startButtonBg.setOutlineColor(sf::Color::White);
        }

        void updateLayout(sf::Vector2u windowSize)
        {
            sf::FloatRect titleBounds = m_title.getLocalBounds();
            m_title.setPosition({windowSize.x / 2.f - titleBounds.size.x / 2.f, windowSize.y / 3.f});

            m_startButtonBg.setPosition({windowSize.x / 2.f - 125.f, windowSize.y / 2.f});

            m_startButton.setPosition({windowSize.x / 2.f - 100.f, windowSize.y / 2.f + 10.f});
        }

        bool isStartButtonClicked(sf::Vector2i mousePos) const
        {
            return m_startButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }

        void render(sf::RenderWindow& window)
        {
            window.setView(window.getDefaultView());
            window.draw(m_title);
            window.draw(m_startButtonBg);
            window.draw(m_startButton);
        }

        void handleHover(sf::Vector2i mousePos)
        {
            if(isStartButtonClicked(mousePos))
            {
                m_startButtonBg.setFillColor(sf::Color(80, 80, 200));
            }
            else
            {
                m_startButtonBg.setFillColor(sf::Color(50, 50, 150));
            }
        }
};

#endif
