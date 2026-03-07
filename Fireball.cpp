#include "Fireball.h"
#include "Entity.h"
#include "PhysicalObject.h"
#include "globals.h"
#include <cmath>

        // // face toward the player
        // sf::Vector2f pPosition = m_player->getPosition();
        // sf::Vector2f delta = pPosition - m_position;
        //
        // // adjust for wrapping
        // if(std::abs(delta.x) > worldWidth / 2.0f)
        //     delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
        // if(std::abs(delta.y) > worldHeight / 2.0f)
        //     delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
        // float angle = std::atan2(delta.y, delta.x);
        //
        // setRotation(angle + M_PI);
        // m_sprite.setRotation(sf::radians(angle + M_PI/2));

extern sf::Vector2f getClosestWrapPosition(const sf::Vector2f& myPos, const sf::Vector2f& otherPos);
extern void wrapPosition(sf::Vector2f& position);
extern std::vector<sf::Vector2f> getDupPositions(const sf::Vector2f& position, float radius);

        // PhysicalObject(sf::Vector2f position, sf::Vector2i size, float rotation, RenderLayer renderLayer, sf::Texture* texture, 
        //         float mass, float radius, sf::Vector2f velocity, float acceleration, float angularVelocity, float maxVelocity, float drag)

Fireball::Fireball(sf::Vector2f startPos, sf::Vector2f targetPos, int damage, float speed, float range)
    : PhysicalObject(startPos, {20, 20}, 0, RenderLayer::Main, &fireballTexture,
            100, 10, {speed, 0}, 0, 0, speed, 0)
    , m_damage(damage)
    , m_hasHit(false)
{
    // Calculate direction to target (accounting for wrapping)
    sf::Vector2f closestTarget = getClosestWrapPosition(startPos, targetPos);
    sf::Vector2f delta = closestTarget - m_position;
    // adjust for wrapping
    if(std::abs(delta.x) > worldWidth / 2.0f)
        delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
    if(std::abs(delta.y) > worldHeight / 2.0f)
        delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
    float angle = std::atan2(delta.y, delta.x);
    setRotation(angle + M_PI);
    m_sprite.setRotation(sf::radians(angle + M_PI/2));

    setVelocity({static_cast<float>(speed * cos(angle)), static_cast<float>(speed * sin(angle))});

    // Calculate time to live based on range
    m_timeToLive = range / speed;

    // Setup sprite
    m_sprite.setTexture(fireballTexture);
    m_sprite.setOrigin({m_size.x / 2.f, m_size.y / 2.f});
}

void Fireball::update()
{
    m_timeToLive -= FixedDeltaTime;
    updatePosition(FixedDeltaTime);
}

void Fireball::draw(sf::RenderWindow& window)
{
    PhysicalObject::draw(window);
    // sf::View view = window.getView();
    // sf::Vector2f viewCenter = view.getCenter();
    // sf::Vector2f viewSize = view.getSize();
    //
    // auto isOnScreen = [&](const sf::Vector2f& pos) {
    //     float dx = std::abs(pos.x - viewCenter.x);
    //     float dy = std::abs(pos.y - viewCenter.y);
    //     return (dx < viewSize.x * 0.6f && dy < viewSize.y * 0.6f);
    // };
    //
    // std::vector<sf::Vector2f> positions = getDupPositions(m_position, m_radius);
    // positions.insert(positions.begin(), m_position);
    //
    // for (const auto& pos : positions) {
    //     if (isOnScreen(pos)) {
    //         m_sprite.setPosition(pos);
    //         window.draw(m_sprite);
    //     }
    // }
}

void Fireball::dealDamage(Entity* entity)
{
    if (!m_hasHit && entity) {
        entity->reduceHealth(m_damage);
        m_hasHit = true;
    }
}

bool Fireball::shouldBeDestroyed() const
{
    return m_timeToLive <= 0 || m_hasHit;
}

void Fireball::markForDestruction()
{
    m_timeToLive = 0;
}
