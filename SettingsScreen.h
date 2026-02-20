#ifndef SETTINGSSCREEN_H
#define SETTINGSSCREEN_H

#include "Resolution.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "globals.h"
#include <iostream>

class SettingsScreen {
    private:
        sf::Text m_title;
        sf::Text m_resolutionText;
        sf::Text m_backButton;
        sf::RectangleShape m_backButtonBg;

        std::vector<sf::Text> m_resolutionOptions;
        std::vector<sf::RectangleShape> m_resolutionBgs;

        ResolutionManager &m_resolutionManager;
        size_t m_hoveredOption;

        const float titleHeight = 0.15f;
        const float titlePosition = 0.1f;

        const float categoryHeight = 0.05f;
        const float resoPosX = 0.1f;

        const float buttonHeight = 0.04f;
        const float buttonWidth = 0.2f;
        const float buttonMarginX = 0.01f; // relative
        const float buttonMarginY = 0.1f; // relative

        const float backButtonPosX = 0.1f;

        const float optionButtonPosX = 0.1f;
        const float optionButtonSpacing = 0.005f;
        float optionStartPosY = 0.35f;

      public:
        SettingsScreen(ResolutionManager &resManager)
            : m_title(sf::Text(font))
              , m_resolutionText(sf::Text(font))
              , m_backButton(sf::Text(font))
              , m_resolutionManager(resManager)
              , m_hoveredOption(-1)
        {
            // title
            m_title.setString("Settings");
            m_title.setCharacterSize(0);
            m_title.setFillColor(sf::Color::White);

            // Resolution label
            m_resolutionText.setString("Resolution:");
            m_resolutionText.setFillColor(sf::Color::White);

            // Back button
            m_backButton.setString("Back");
            m_backButton.setFillColor(sf::Color::White);

            m_backButtonBg.setFillColor(sf::Color(150, 50, 50));
            m_backButtonBg.setOutlineThickness(2);
            m_backButtonBg.setOutlineColor(sf::Color::White);

            // Create resolution options
            const auto &resolutions = m_resolutionManager.getAllResolutions();
            for (size_t i = 0; i < resolutions.size(); i++)
            {
                sf::Text option(font);
                option.setString(resolutions[i].displayName);
                option.setCharacterSize(0);
                option.setFillColor(sf::Color::White);
                m_resolutionOptions.push_back(option);

                sf::RectangleShape bg({windowWidth * optionButtonPosX, buttonHeight});
                bg.setFillColor(sf::Color(50, 50, 50));
                bg.setOutlineThickness(2);
                bg.setOutlineColor(sf::Color(100, 100, 100));
                m_resolutionBgs.push_back(bg);
            }
        }

        void updateLayout(sf::Vector2u windowSize)
        {
        // void updateLayout(sf::RenderWindow& window)
        // {
            // sf::Vector2u windowSize = {static_cast<unsigned int>(window.getView().getSize().x), static_cast<unsigned int>(window.getView().getSize().x)};
            std::cout << "Current window size: (" << windowSize.x << ", " << windowSize.y << ")" << std::endl;

            // Title
            m_title.setCharacterSize(windowSize.y * titleHeight);
            sf::FloatRect titleBounds = m_title.getLocalBounds();
            m_title.setPosition({windowSize.x / 2.f - titleBounds.size.x / 2.f, 50.f});

            // Resolution label
            m_resolutionText.setCharacterSize(windowSize.y * categoryHeight);
            m_resolutionText.setPosition({windowSize.x * resoPosX, (windowSize.y * optionStartPosY)
                    - (m_resolutionText.getCharacterSize()) - ((windowSize.y * buttonHeight) * buttonMarginY)});

            // Resolution options
            float startY = windowSize.y * optionStartPosY;
            for (size_t i = 0; i < m_resolutionOptions.size(); i++)
            {
                m_resolutionBgs[i].setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
                m_resolutionBgs[i].setPosition({windowSize.x * optionButtonPosX, startY});
                m_resolutionOptions[i].setCharacterSize(m_resolutionBgs[i].getSize().y * (1 - buttonMarginY));
                m_resolutionOptions[i].setPosition({windowSize.x * optionButtonPosX, (startY + ((m_resolutionBgs[i].getSize().y * buttonMarginY) / 2.f))});

                startY += (windowSize.y * buttonHeight) + (windowSize.y * optionButtonSpacing);
            }

            m_backButtonBg.setSize({(windowSize.x * buttonWidth), (windowSize.y * buttonHeight)});
            m_backButtonBg.setPosition({windowSize.x * backButtonPosX, startY});
            m_backButton.setCharacterSize(m_backButtonBg.getSize().y * (1 - buttonMarginY));
            m_backButton.setPosition({windowSize.x * backButtonPosX, (startY + ((m_backButtonBg.getSize().y * buttonMarginY) / 2.f))});
        }

        int getHoveredResolution(sf::Vector2i mousePos) const
        {
            for (size_t i = 0; i < m_resolutionBgs.size(); i++)
            {
                if (m_resolutionBgs[i].getGlobalBounds().contains(
                            static_cast<sf::Vector2f>(mousePos)))
                {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

        bool isBackHovered(sf::Vector2i mousePos) const
        {
            if(mousePos.x >= m_backButtonBg.getPosition().x && mousePos.x <= m_backButtonBg.getPosition().x + m_backButtonBg.getSize().x)
            {
                if(mousePos.y >= m_backButtonBg.getPosition().y && mousePos.y <= m_backButtonBg.getPosition().y + m_backButtonBg.getSize().y)
                {
                    return true;
                }
            }
            return false;


            return m_backButtonBg.getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(mousePos));
        }

        void handleHover(sf::Vector2i mousePos)
        {
            // Reset all colors
            for (auto &bg : m_resolutionBgs)
            {
                bg.setFillColor(sf::Color(50, 50, 50));
            }

            if (isBackHovered(mousePos)) {
                m_backButtonBg.setFillColor(sf::Color(200, 80, 80));
            } else {
                m_backButtonBg.setFillColor(sf::Color(150, 50, 50));
            }

            // Highlight hovered resolution
            int hovered = getHoveredResolution(mousePos);
            if (hovered >= 0) {
                m_resolutionBgs[hovered].setFillColor(sf::Color(80, 80, 80));
            }

            // Highlight current resolution
            size_t current = m_resolutionManager.getCurrentIndex();
            m_resolutionBgs[current].setFillColor(sf::Color(50, 100, 150));
        }

        void render(sf::RenderWindow &window)
        {
            window.setView(window.getDefaultView());

            window.draw(m_title);
            window.draw(m_resolutionText);

            for (size_t i = 0; i < m_resolutionOptions.size(); ++i)
            {
                window.draw(m_resolutionBgs[i]);
                window.draw(m_resolutionOptions[i]);
            }

            window.draw(m_backButtonBg);
            window.draw(m_backButton);
        }
};

#endif
