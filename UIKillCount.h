#ifndef UIKILLCOUNT_H
#define UIKILLCOUNT_H

#include "UIElement.h"

class UIKillCount : public UIElement
{
    protected:
        int *m_killCount;
        sf::Text m_text;

        float textPositionXFraction = 0.015;
        float textPositionYFraction = 0.01;

        float textHeightFraction = 0.04;

        sf::Color textColor = sf::Color::White;

    public:
        UIKillCount(std::shared_ptr<Player> player);
        UIKillCount(const UIKillCount& other); // copy constructor
        UIKillCount(UIKillCount&& other) noexcept; // move constructor

        int getKillCount() const;
        void setKillCount(int count);

        virtual void update(sf::RenderWindow& window) override;
        virtual void render(sf::RenderWindow& window) override;
};

#endif
