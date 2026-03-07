#include "CircleWeapon.h"
#include "Entity.h"
#include "Player.h"

extern bool detectIntersection(const sf::Vector2f& pos1, float size1, const sf::Vector2f& pos2, float size2);

// Weapon(int damage, float cooldown, float size, sf::Vector2f velocity, float duration, float kb , float timeSince, bool active)

CircleWeapon::CircleWeapon()
    : Weapon(10, 0.25, 100, {0, 0}, 0.1, 0, 0, false)
    , m_visualTimer(0.f)
    , m_visualDuration(m_cooldown) // visual effect always on
{
    Weapon::setName("CircleWeapon");
    m_visualEffect.setRadius(m_size);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 80));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({static_cast<float>(m_size), static_cast<float>(m_size)});
}
CircleWeapon::CircleWeapon(int damage, float cooldown, float size)
    : Weapon(damage, cooldown, size, {0, 0}, 0.1, 0, 0, false)
    , m_visualTimer(0.f)
    , m_visualDuration(0.25f)
{
    Weapon::setName("CircleWeapon");
    m_visualEffect.setRadius(size);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 255));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({static_cast<float>(m_size), static_cast<float>(m_size)});
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
    , m_visualTimer(other.m_visualTimer)
    , m_visualDuration(other.m_visualDuration)
{
    Weapon::setName("CircleWeapon");
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
    : Weapon(std::move(other))
    , m_visualTimer(other.m_visualTimer)
    , m_visualDuration(other.m_visualDuration)
{
    Weapon::setName("CircleWeapon");
}

void CircleWeapon::activate(Player& player, QuadTree& quadTree)
{
    // visuals
    m_visualTimer = m_visualDuration;

    // get nearby enemies to damage
    std::vector<Entity*> nearbyEntities;
    quadTree.retrieveEntities(nearbyEntities, player.getPosition(), m_size);

    for(auto& entity : nearbyEntities)
    {
        if(entity == &player) continue;

        bool overlap = detectIntersection(entity->getPosition(), entity->getRadius(),
                player.getPosition(), getSize());

        if(overlap)
        {
            entity->reduceHealth(m_damage);
        }
    }
}

void CircleWeapon::update(float deltaTime, Player& player, QuadTree& quadTree)
{
    Weapon::update(deltaTime, player, quadTree);
    m_visualEffect.setPosition(player.getPosition());
    if(m_visualTimer > 0)
    {
        m_visualTimer -= deltaTime;
    }
}

void CircleWeapon::render(sf::RenderWindow& window)
{
    // std::cout << "Rendering weapon, timer: " << m_visualTimer << std::endl;
    if(m_visualTimer > 0)
        window.draw(m_visualEffect);
}

