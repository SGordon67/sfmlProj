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

        const float buttonMarginX = 1.2f;
        const float buttonMarginY = 1.5f;

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

        void updateLayout(sf::Vector2u windowSize) {

            // text sizes
            m_title.setCharacterSize(windowSize.y / 5);
            m_startButtonText.setCharacterSize(windowSize.y / 10);
            m_settingsButtonText.setCharacterSize(windowSize.y / 10);

            // text position and bg size/position
            sf::FloatRect titleBounds = m_title.getLocalBounds();
            m_title.setPosition({((windowSize.x / 2.f) - (titleBounds.size.x / 2.f)), 
                    ((windowSize.y / 3.f) - (titleBounds.size.y / 2.f))});

            m_startButtonText.setPosition({((windowSize.x / 2.f) - (m_startButtonText.getLocalBounds().size.x / 2.f)), 
                    (((windowSize.y / 3.f) * 2.f) - (m_startButtonText.getLocalBounds().size.y / 2.f))});
            sf::FloatRect startTextBounds = m_startButtonText.getGlobalBounds();
            m_startButtonBg.setSize({startTextBounds.size.x * buttonMarginX, startTextBounds.size.y * buttonMarginY});
            m_startButtonBg.setPosition({startTextBounds.position.x - startTextBounds.size.x * ((buttonMarginX - 1) / 2.f), 
                    startTextBounds.position.y - startTextBounds.size.y * ((buttonMarginY - 1) / 2.f)});

            m_settingsButtonText.setPosition({m_startButtonText.getPosition().x, 
                    m_startButtonBg.getPosition().y + m_startButtonBg.getSize().y + 2 * buttonMarginY});
            sf::FloatRect settingsTextBounds = m_settingsButtonText.getGlobalBounds();
            m_settingsButtonBg.setSize({settingsTextBounds.size.x * buttonMarginX, settingsTextBounds.size.y * buttonMarginY});
            m_settingsButtonBg.setPosition({settingsTextBounds.position.x - settingsTextBounds.size.x * ((buttonMarginX - 1) / 2.f), 
                    settingsTextBounds.position.y - settingsTextBounds.size.y * ((buttonMarginY - 1) / 2.f)});
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
