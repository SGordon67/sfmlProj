#include "FireballWeapon.h"
#include "Entity.h"
#include "Player.h"

extern bool detectIntersection(const sf::Vector2f& pos1, float size1, const sf::Vector2f& pos2, float size2);

// Weapon(int damage, float cooldown, float size, sf::Vector2f velocity, float duration, float kb , float timeSince, bool active)

FireballWeapon::FireballWeapon()
    : Weapon(10, 0.25, 100, {0, 0}, 0.1, 0, 0, false)
    , m_visualTimer(0.f)
    , m_visualDuration(m_cooldown) // visual effect always on
{
    Weapon::setName("FireballWeapon");
    m_visualEffect.setRadius(m_size);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 80));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({m_size, m_size});
}
FireballWeapon::FireballWeapon(int damage, float cooldown, float size)
    : Weapon(damage, cooldown, size, {0, 0}, 0.1, 0, 0, false)
    , m_visualTimer(0.f)
    , m_visualDuration(0.25f)
{
    Weapon::setName("FireballWeapon");
    m_visualEffect.setRadius(size);
    m_visualEffect.setFillColor(sf::Color(255, 100, 100, 255));
    m_visualEffect.setOutlineThickness(3);
    m_visualEffect.setOutlineColor(sf::Color::Red);
    m_visualEffect.setOrigin({m_size, m_size});
}
FireballWeapon::FireballWeapon(const FireballWeapon& other) // copy constructor
	: Weapon(other)
    , m_visualTimer(other.m_visualTimer)
    , m_visualDuration(other.m_visualDuration)
{
    Weapon::setName("FireballWeapon");
}
FireballWeapon::FireballWeapon(FireballWeapon&& other) noexcept // move constructor
    : Weapon(std::move(other))
    , m_visualTimer(other.m_visualTimer)
    , m_visualDuration(other.m_visualDuration)
{
    Weapon::setName("FireballWeapon");
}

void FireballWeapon::activate(Player& player, QuadTree& quadTree)
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

void FireballWeapon::update(float deltaTime, Player& player, QuadTree& quadTree)
{
    Weapon::update(deltaTime, player, quadTree);
    m_visualEffect.setPosition(player.getPosition());
    if(m_visualTimer > 0)
    {
        m_visualTimer -= deltaTime;
    }
}

void FireballWeapon::render(sf::RenderWindow& window)
{
    // std::cout << "Rendering weapon, timer: " << m_visualTimer << std::endl;
    if(m_visualTimer > 0)
        window.draw(m_visualEffect);
}


