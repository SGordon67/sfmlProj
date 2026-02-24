#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "QuadTree.h"
#include "Weapon.h"

class Player : public Entity
{
    private:
        // defaults
        static constexpr sf::Vector2f d_playerPosition = {900, 500}; // default sorta near the middle of the game
        // static constexpr sf::Vector2f d_playerPosition = {35, 35}; // testing damage against spikeys in corners
        static constexpr sf::Vector2i d_playerSize = {24, 30};
        static constexpr int d_playerMass = 10;
        static constexpr float d_playerRadius = d_playerSize.x/2.f;
        static constexpr sf::Vector2f d_playerVelocity = {0, 0};
        static constexpr float d_playerAcceleration = 5000;
        static constexpr float d_playerAngularVelocity = 0;
        static constexpr float d_playerMaxVelocity = 500;
        static constexpr float d_playerDrag = 4;

        static constexpr int d_playerHP = 10;
        static constexpr int d_playerMaxHP = 10;
        static constexpr float d_playerAngularAcceleration = (4.f * (M_PI / 180.f));

        float m_angularAcceleration;
        std::array<bool, (size_t)Button::Count> m_buttons{false};
        std::array<std::unique_ptr<Weapon>, 4> m_weapons;

    public:
        Player(std::vector<std::unique_ptr<VisualObject>>* m_visualObjects);
        Player(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
                float mass, float radius, sf::Vector2f velocity, float acceleration, float rotationVelocity, float maxVelocity, float drag,
                int hp, int maxHP, std::vector<std::unique_ptr<VisualObject>>* m_visualObjects, float angularAccleration);

        float getAngularAcceleration();
        void setAngularAcceleration(float angularAcceleration);

        void updateButtonPresses();
        bool isPressed(Button button) const;
        void setPressed(Button button, bool value);

        void updateWeapons(float deltaTime, QuadTree& quadTree);
        void renderWeapons(sf::RenderWindow& window);
        void addWeapon(std::unique_ptr<Weapon> weapon, int slot);

        void updateRotation() override;
        void printInfo();
        void playerUpdate();
        virtual void update() override;
};

#endif
