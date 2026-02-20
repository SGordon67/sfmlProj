#ifndef MAINMENU_H
#define MAINMENU_H

#include "SFML/Graphics/RectangleShape.hpp"
#include "globals.h"
#include <SFML/Graphics.hpp>

class MainMenu {
    private:
        sf::Text m_title;

        sf::Text m_startButtonText;
        sf::RectangleShape m_startButtonBg;

        sf::Text m_settingsButtonText;
        sf::RectangleShape m_settingsButtonBg;

        const float buttonFirstPosY = 0.5f;
        const float buttonMargin = 0.01f;

        const float buttonMarginX = 0.01f; // relative
        const float buttonMarginY = 0.05f; // relative
        const float buttonWidth = 0.35f;
        const float buttonHeight = 0.04f;

    public:
        MainMenu() 
            : m_title(sf::Text(font))
              , m_startButtonText(sf::Text(font))
              , m_settingsButtonText(sf::Text(font))
        {
            m_title.setString("Meteorite");
            m_title.setFillColor(sf::Color::White);

            m_startButtonText.setString("Start Game");
            m_startButtonText.setFillColor(sf::Color::White);

            m_settingsButtonText.setString("Settings");
            m_settingsButtonText.setFillColor(sf::Color::White);

            m_startButtonBg.setFillColor(sf::Color(50, 50, 150));
            m_startButtonBg.setOutlineThickness(3);
            m_startButtonBg.setOutlineColor(sf::Color::White);

            m_settingsButtonBg.setFillColor(sf::Color(50, 50, 150));
            m_settingsButtonBg.setOutlineThickness(3);
            m_settingsButtonBg.setOutlineColor(sf::Color::White);
        }

        void updateLayout(sf::Vector2u windowSize)
        {
            // in case of window resize
            // m_fullRect.setSize(sf::Vector2f(currentViewWidth * barWidthFraction, currentViewHeight * barHeightFraction));
            // m_fullRect.setPosition(sf::Vector2f(currentViewWidth * xMarginFraction, currentViewHeight * yMarginFraction));

            m_title.setCharacterSize(windowSize.y * 0.1f);
            m_title.setPosition({static_cast<float>(windowSize.x * 0.1), static_cast<float>(windowSize.y * 0.1)});



            // m_title.setCharacterSize(windowSize.y / 5);
            // m_title.setPosition({((windowSize.x / 2.f) - (m_title.getGlobalBounds().size.length() / 2.f)), 
            //         ((windowSize.y / 3.f) - (m_title.getCharacterSize() / 2.f))});

            m_startButtonText.setCharacterSize(windowSize.y / 10);
            m_startButtonBg.setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
            m_startButtonBg.setPosition({(windowSize.x / 2.f) - (m_startButtonBg.getSize().x / 2.f), 
                    (windowSize.y * buttonFirstPosY)});
            sf::FloatRect startButtonBounds = m_startButtonBg.getGlobalBounds();
            m_startButtonText.setCharacterSize(startButtonBounds.size.y * (1 - buttonMarginY));
            m_startButtonText.setPosition({(windowSize.x / 2.f) - (m_startButtonText.getLocalBounds().size.x / 2.f), 
                    (startButtonBounds.position.y - (m_startButtonBg.getSize().y * buttonMarginY))});

            m_settingsButtonText.setCharacterSize(windowSize.y / 10);
            m_settingsButtonBg.setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
            m_settingsButtonBg.setPosition({(windowSize.x / 2.f) - (m_settingsButtonBg.getSize().x / 2.f), 
                    (windowSize.y * buttonFirstPosY + (1 * ((windowSize.y * buttonHeight) + (windowSize.y * buttonMargin))))}); // 1 for adding one button position
            sf::FloatRect settingsButtonBounds = m_settingsButtonBg.getGlobalBounds();
            m_settingsButtonText.setCharacterSize(settingsButtonBounds.size.y * (1 - buttonMarginY));
            m_settingsButtonText.setPosition({(windowSize.x / 2.f) - (m_settingsButtonText.getLocalBounds().size.x / 2.f), 
                    (settingsButtonBounds.position.y - (m_settingsButtonBg.getSize().y * buttonMarginY))});
        }

        bool isStartButtonHovered(sf::Vector2i mousePos) const
        {
            return m_startButtonBg.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }
        bool isSettingsButtonHovered(sf::Vector2i mousePos) const
        {
            return m_settingsButtonBg.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
        }

        void render(sf::RenderWindow &window) {
            window.setView(window.getDefaultView());
            window.draw(m_title);
            window.draw(m_startButtonBg);
            window.draw(m_startButtonText);
            window.draw(m_settingsButtonBg);
            window.draw(m_settingsButtonText);
        }

        void handleHover(sf::Vector2i mousePos) {
            if (isStartButtonHovered(mousePos)) {
                m_startButtonBg.setFillColor(sf::Color(80, 80, 200));
            } else {
                m_startButtonBg.setFillColor(sf::Color(50, 50, 150));
            }

            if (isSettingsButtonHovered(mousePos)) {
                m_settingsButtonBg.setFillColor(sf::Color(80, 80, 200));
            } else {
                m_settingsButtonBg.setFillColor(sf::Color(50, 50, 150));
            }
        }
};

#endif
