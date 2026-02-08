#include "CircleWeapon.h"
#include "Entity.h"
#include "Player.h"

extern bool detectIntersection(const sf::Vector2f& pos1, float radius1, const sf::Vector2f& pos2, float radius2);

CircleWeapon::CircleWeapon()
    : Weapon(10, 0.25, 0, false)
      , m_radius(100)
      , m_visualTimer(0.f)
      , m_visualDuration(0.25f)
{
    Weapon::setName("CircleWeapon");
    m_visualEffect.setRadius(m_radius);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 80));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({m_radius, m_radius});
}
CircleWeapon::CircleWeapon(int damage, float cooldown, float radius)
    : Weapon(damage, cooldown, 0, false)
      , m_radius(radius)
{
    Weapon::setName("CircleWeapon");
    m_visualEffect.setRadius(radius);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 255));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({m_radius, m_radius});
}
CircleWeapon::CircleWeapon(const CircleWeapon& other) // copy constructor
	: Weapon(other)
      , m_radius(other.m_radius)
{
    Weapon::setName("CircleWeapon");
}
CircleWeapon::CircleWeapon(CircleWeapon&& other) noexcept // move constructor
    : Weapon(std::move(other))
      , m_radius(other.m_radius)
{
    Weapon::setName("CircleWeapon");
}

float CircleWeapon::getRadius()
{
    return m_radius;
}
void CircleWeapon::setRadius(float radius)
{
    m_radius = radius;
}

void CircleWeapon::activate(Player& player, QuadTree& quadTree)
{
    // visuals
    m_visualTimer = m_visualDuration;

    // get nearby enemies to damage
    std::vector<Entity*> nearbyEntities;
    quadTree.retrieveEntities(nearbyEntities, player.getPosition(), m_radius);

    for(auto& entity : nearbyEntities)
    {
        if(entity == &player) continue;

        bool overlap = detectIntersection(entity->getPosition(), entity->getRadius(),
                player.getPosition(), getRadius());

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

