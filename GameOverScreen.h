#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "globals.h"
#include <SFML/Graphics.hpp>

class GameOverScreen
{
    private:
        sf::Text m_gameOverText;

        const float buttonFirstPosY = 0.7f;
        const float buttonMargin = 0.01f;

        const float buttonMarginX = 0.01f; // relative
        const float buttonMarginY = 0.05f; // relative
        const float buttonWidth = 0.35f;
        const float buttonHeight = 0.04f;

        sf::Text m_restartButton;
        sf::RectangleShape m_restartBg;

        sf::Text m_menuButton;
        sf::RectangleShape m_menuBg;
    public:
        GameOverScreen()
            : m_gameOverText(sf::Text(font))
              , m_restartButton(sf::Text(font))
              , m_menuButton(sf::Text(font))
        {
            m_gameOverText.setString("Game Over!");
            m_gameOverText.setCharacterSize(64);
            m_gameOverText.setFillColor(sf::Color::White);

            m_restartButton.setString("Restart");
            m_restartButton.setCharacterSize(32);
            m_restartButton.setFillColor(sf::Color::White);

            m_menuButton.setString("Main Menu");
            m_menuButton.setCharacterSize(32);
            m_menuButton.setFillColor(sf::Color::White);

            m_restartBg.setSize({200, 50});
            m_restartBg.setFillColor(sf::Color(50, 50, 150));
            m_restartBg.setOutlineThickness(2);
            m_restartBg.setOutlineColor(sf::Color::White);

            m_menuBg.setSize({200, 50});
            m_menuBg.setFillColor(sf::Color(50, 50, 150));
            m_menuBg.setOutlineThickness(2);
            m_menuBg.setOutlineColor(sf::Color::White);
        }

        void updateLayout(sf::Vector2u windowSize)
        {
            sf::FloatRect textBounds = m_gameOverText.getLocalBounds();
            m_gameOverText.setPosition({windowSize.x / 2.f - textBounds.size.x / 2.f, windowSize.y / 5.f});

            m_restartButton.setCharacterSize(windowSize.y / 10);
            m_restartBg.setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
            m_restartBg.setPosition({(windowSize.x / 2.f) - (m_restartBg.getSize().x / 2.f), 
                    (windowSize.y * buttonFirstPosY)});
            sf::FloatRect startButtonBounds = m_restartBg.getGlobalBounds();
            m_restartButton.setCharacterSize(startButtonBounds.size.y * (1 - buttonMarginY));
            m_restartButton.setPosition({(windowSize.x / 2.f) - (m_restartButton.getLocalBounds().size.x / 2.f), 
                    (startButtonBounds.position.y - (m_restartBg.getSize().y * buttonMarginY))});

            m_menuButton.setCharacterSize(windowSize.y / 10);
            m_menuBg.setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
            m_menuBg.setPosition({(windowSize.x / 2.f) - (m_menuBg.getSize().x / 2.f), 
                    (windowSize.y * buttonFirstPosY + (1 * ((windowSize.y * buttonHeight) + (windowSize.y * buttonMargin))))}); // 1 for adding one button position
            sf::FloatRect settingsButtonBounds = m_menuBg.getGlobalBounds();
            m_menuButton.setCharacterSize(settingsButtonBounds.size.y * (1 - buttonMarginY));
            m_menuButton.setPosition({(windowSize.x / 2.f) - (m_menuButton.getLocalBounds().size.x / 2.f), 
                    (settingsButtonBounds.position.y - (m_menuBg.getSize().y * buttonMarginY))});

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
            if (isMenuButtonHovered(mousePos))
            {
                m_menuBg.setFillColor(sf::Color(80, 80, 200));
            } else
            {
                m_menuBg.setFillColor(sf::Color(50, 50, 150));
            }

            if (isRestartButtonHovered(mousePos))
            {
                m_restartBg.setFillColor(sf::Color(80, 80, 200));
            } else
            {
                m_restartBg.setFillColor(sf::Color(50, 50, 150));
            }
        }
};

#endif
