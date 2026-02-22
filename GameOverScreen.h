#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "globals.h"
#include <SFML/Graphics.hpp>

class GameOverScreen
{
    private:
        sf::Text m_gameOverText;
        sf::Text m_restartButton;
        sf::Text m_menuButton;
        sf::RectangleShape m_restartBg;
        sf::RectangleShape m_menuBg;

    public:
        GameOverScreen()
            : m_gameOverText(sf::Text(font))
              , m_restartButton(sf::Text(font))
              , m_menuButton(sf::Text(font))
        {
            m_gameOverText.setString("Game Over!");
            m_gameOverText.setCharacterSize(64);
            m_gameOverText.setFillColor(sf::Color::Red);

            m_restartButton.setString("Restart");
            m_restartButton.setCharacterSize(32);
            m_restartButton.setFillColor(sf::Color::Red);

            m_menuButton.setString("Main Menu");
            m_menuButton.setCharacterSize(32);
            m_menuButton.setFillColor(sf::Color::Red);

            m_restartBg.setSize({200, 50});
            m_restartBg.setFillColor(sf::Color::Green);
            m_restartBg.setOutlineThickness(2);
            m_restartBg.setOutlineColor(sf::Color::White);

            m_menuBg.setSize({200, 50});
            m_menuBg.setFillColor(sf::Color::Green);
            m_menuBg.setOutlineThickness(2);
            m_menuBg.setOutlineColor(sf::Color::White);
        }

        void updateLayout(sf::Vector2u windowSize)
        {
            sf::FloatRect textBounds = m_gameOverText.getLocalBounds();
            m_gameOverText.setPosition({windowSize.x / 2.f - textBounds.size.x / 2.f, windowSize.y / 3.f});

            m_restartBg.setPosition({windowSize.x / 2.f - 100.f, windowSize.y / 2.f});
            m_restartButton.setPosition({windowSize.x / 2.f - 80.f, windowSize.y / 2.f + 10.f});

            m_menuBg.setPosition({windowSize.x / 2.f - 100.f, windowSize.y / 2.f + 80.f});
            m_menuButton.setPosition({windowSize.x / 2.f - 80.f, windowSize.y / 2.f + 90.f});
        }

        bool isRestartButtonHovered(sf::Vector2i mousePos) const
        {
            return m_restartBg.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }
        bool isMenuButtonHovered(sf::Vector2i mousePos) const
        {
            return m_menuBg.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }

        void render(sf::RenderWindow& window)
        {
            window.draw(m_gameOverText);
            window.draw(m_restartBg);
            window.draw(m_restartButton);
            window.draw(m_menuBg);
            window.draw(m_menuButton);
        }

        void handleHover(sf::Vector2i mousePos)
        {
            if(isMenuButtonHovered(mousePos))
            {
                m_menuBg.setFillColor(sf::Color::Blue);
            }
            else
            {
                m_menuBg.setFillColor(sf::Color::Green);
            }

            if(isRestartButtonHovered(mousePos))
            {
                m_restartBg.setFillColor(sf::Color::Blue);
            }
            else
            {
                m_restartBg.setFillColor(sf::Color::Green);
            }
        }
};

#endif
