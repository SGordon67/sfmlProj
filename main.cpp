// TODO aspect ratio

#include "Minimap.h"
#include "QuadTree.h"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <random>
#include <vector>

#include "UIElement.h"
#include "UIHealth.h"
#include "globals.h"
#include "enums.h"
#include "BasicObject.h"
#include "VisualObject.h"
#include "PhysicalObject.h"
#include "Ball.h"
#include "Player.h"
#include "Spikey.h"
#include "Enemy1.h"
#include "HealthCrate.h"

float degreesToRadians(float degrees)
{
    return (degrees * (M_PI / 180.f));
}

float radiansToDegrees(float radians)
{
    return (radians * (180.f / M_PI));
}

void wrapPosition(sf::Vector2f& position)
{
    position.x = std::fmod(position.x, worldWidth);
    position.y = std::fmod(position.y, worldHeight);

    if(position.x < 0) position.x += worldWidth;
    if(position.y < 0) position.y += worldHeight;
}

sf::Vector2f getClosestWrapPosition(const sf::Vector2f& myPosition, const sf::Vector2f& otherPosition)
{
    // shortest distance with wrapping
    sf::Vector2f ClosestPosition = otherPosition;
    sf::Vector2f delta = otherPosition - myPosition;
    if(std::abs(delta.x) > worldWidth / 2.0f)
        ClosestPosition.x = otherPosition.x + (delta.x > 0 ? (-worldWidth) : worldWidth);
    if(std::abs(delta.y) > worldHeight / 2.0f)
        ClosestPosition.y = otherPosition.y + (delta.y > 0 ? (-worldHeight) : worldHeight);
    return ClosestPosition;
}

void initializeButtons()
{
    buttons[0] = &escPressed;
    buttons[1] = &upPressed;
    buttons[2] = &downPressed;
    buttons[3] = &leftPressed;
    buttons[4] = &rightPressed;
    buttons[5] = &interactPressed;
    buttons[6] = &tabPressed;
}

void initializeTextures()
{
    if(!farBackgroundTexture.loadFromFile("art/basicBackground.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    farBackgroundTexture.setRepeated(true);
    if(!closeBackgroundTexture.loadFromFile("art/basicStars.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    closeBackgroundTexture.setRepeated(true);
    if(!meteorTexture.loadFromFile("art/basicMeteor.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!crateTexture.loadFromFile("art/basicCrate.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!spikeyTexture.loadFromFile("art/basicSpikey.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!playerTexture.loadFromFile("art/basicSpriteL.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
    if(!enemyTexture.loadFromFile("art/basicEnemy.png"))
    {
        std::cout << "Sprite not loaded :(" << std::endl;
    }
}

bool detectIntersection(const sf::Vector2f& pos1, float radius1, const sf::Vector2f& pos2, float radius2)
{
    // shortest distance with wrapping

    sf::Vector2f closestPos = getClosestWrapPosition(pos1, pos2);
    sf::Vector2f delta = pos1 - closestPos;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    if(distance <= (radius1 + radius2))
        return true;
    return false;
}

void detectAndHandleInteractions(std::shared_ptr<Player> player, const std::vector<std::shared_ptr<Interactable>>& interactableObjects)
{
    for(auto& obj : interactableObjects)
    {
        bool overlap = detectIntersection(player->getPosition(), player->getRadius(), obj->getPosition(), obj->getInteractionRadius());
        if(overlap)
        {
            obj->interact(player);
            // std::cout << "Interacting with overlapping object: " << player.getObjectID() << ", " << obj->canInteract() << std::endl;
        }
    }
}

// std::vector<sf::Vector2f> getDupPositions(const sf::Vector2f& position, const sf::Vector2f& size)
std::vector<sf::Vector2f> getDupPositions(const sf::Vector2f& position, float radius)
{
    std::vector<sf::Vector2f> dupPositions = {};

    // float wrapXThreshold = (std::abs(worldWidth - (viewWidth / 2.f)) + size.x);
    // float wrapYThreshold = (std::abs(worldHeight - (viewHeight / 2.f)) + size.y);
    float wrapXThreshold = radius;
    float wrapYThreshold = radius;

    // need to replicate near the boundaries
    bool nearLeft = (position.x < wrapXThreshold);
    bool nearRight = (position.x > worldWidth - wrapXThreshold);
    bool nearTop = (position.y < wrapYThreshold);
    bool nearBottom = (position.y > worldHeight - wrapYThreshold);

    if(nearLeft) dupPositions.push_back(position + sf::Vector2f{static_cast<float>(worldWidth), 0});
    if(nearRight) dupPositions.push_back(position + sf::Vector2f{-static_cast<float>(worldWidth), 0});
    if(nearTop) dupPositions.push_back(position + sf::Vector2f{0, static_cast<float>(worldHeight)});
    if(nearBottom) dupPositions.push_back(position + sf::Vector2f{0, -static_cast<float>(worldHeight)});

    if(nearLeft && nearTop) dupPositions.push_back(position + sf::Vector2f{static_cast<float>(worldWidth), static_cast<float>(worldHeight)});
    if(nearLeft && nearBottom) dupPositions.push_back(position + sf::Vector2f{static_cast<float>(worldWidth), -static_cast<float>(worldHeight)});

    if(nearRight && nearTop) dupPositions.push_back(position + sf::Vector2f{-static_cast<float>(worldWidth), static_cast<float>(worldHeight)});
    if(nearRight && nearBottom) dupPositions.push_back(position + sf::Vector2f{-static_cast<float>(worldWidth), -static_cast<float>(worldHeight)});

    return dupPositions;
}

void handleCollisionLite(const std::shared_ptr<PhysicalObject>& mainObject, const std::shared_ptr<PhysicalObject>& otherObject)
{
    // should collide with the closest version of the object including wrapped positions
    sf::Vector2f otherClosestPosition = getClosestWrapPosition(mainObject->getPosition(),otherObject->getPosition());
    sf::Vector2f dir = mainObject->getPosition() - otherClosestPosition;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if(dist < mainObject->getRadius() + otherObject->getRadius() && dist > 0)
    {
        dir /= dist;
        float separation = (mainObject->getRadius() + otherObject->getRadius() - dist) / 2.0f;
        mainObject->setPosition(mainObject->getPosition() + dir * separation);
        otherObject->setPosition(otherObject->getPosition() - dir * separation);
    }
}

void handleCollision(std::shared_ptr<PhysicalObject>& mainObject, std::shared_ptr<PhysicalObject>& otherObject,
                     float restitution = 1.0f, float friction = 0.5f)
{
    // varials to keep track of collision information
    sf::Vector2f normal;
    sf::Vector2f contact1;
    sf::Vector2f contact2;
    sf::Vector2f r1;
    sf::Vector2f r2;
    float overlap;

    // Calculate the collision normal
    sf::Vector2f otherClosesetPosition = getClosestWrapPosition(mainObject->getPosition(), otherObject->getPosition());
    sf::Vector2f delta = otherClosesetPosition - mainObject->getPosition();
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    // Avoid 0 edge case
    if (distance == 0.0f) {
        delta = sf::Vector2f(1.0f, 0.0f);
        distance = 1.0f;
    }
    // Normalize collision normal
    normal = delta / distance;
    // Calculate contact points
    contact1 = mainObject->getPosition() + normal * mainObject->getRadius();
    contact2 = otherClosesetPosition - normal * otherObject->getRadius();
    // Radii from center of mass to contact points
    r1 = contact1 - mainObject->getPosition();
    r2 = contact2 - otherClosesetPosition;
    // Calculate overlap
    overlap = (mainObject->getRadius() + otherObject->getRadius()) - distance;

    // Apply normal impulse
    // Calculate velocity at contact points
    sf::Vector2f v1 = mainObject->getVelocity() + 
        sf::Vector2f(-mainObject->getAngularVelocity() * r1.y,
                     mainObject->getAngularVelocity() * r1.x);
    sf::Vector2f v2 = otherObject->getVelocity() + 
        sf::Vector2f(-otherObject->getAngularVelocity() * r2.y,
                     otherObject->getAngularVelocity() * r2.x);
    // Relative velocity
    sf::Vector2f relativeVelocity = v1 - v2;
    // Project velocity onto normal
    float velocityAlongNormal = relativeVelocity.dot(normal);
    // Objects already separating, don't apply impulse
    if (velocityAlongNormal < 0.0f) return;
    // Calculate moment of inertia
    float I1 = 0.05f * mainObject->getMass() * mainObject->getRadius() * mainObject->getRadius();
    float I2 = 0.05f * otherObject->getMass() * otherObject->getRadius() * otherObject->getRadius();
    // Cross products for rotational contributio
    float r1CrossN = r1.x * normal.y - r1.y * normal.x;
    float r2CrossN = r2.x * normal.y - r2.y * normal.x;
    // Calculate impulse scalar
    float impulseScalar = (-(1.0f + restitution) * velocityAlongNormal) /
        ((1.0f / mainObject->getMass()) + (1.0f / otherObject->getMass()) +
        (r1CrossN * r1CrossN / I1) + (r2CrossN * r2CrossN / I2));
    // Apply impulse to linear velocitie
    sf::Vector2f impulse = normal * impulseScalar;
    mainObject->setVelocity(mainObject->getVelocity() + impulse / mainObject->getMass());
    otherObject->setVelocity(otherObject->getVelocity() - impulse / otherObject->getMass());


    // Apply friction impulse for rotation
    // Calculate tangent
    sf::Vector2f tangent(-normal.y, normal.x);

    // Velocity at contact points
    v1 = mainObject->getVelocity() +
        sf::Vector2f(-mainObject->getAngularVelocity() * r1.y,
                     mainObject->getAngularVelocity() * r1.x);
    v2 = otherObject->getVelocity() +
        sf::Vector2f(-otherObject->getAngularVelocity() * r2.y,
                     otherObject->getAngularVelocity() * r2.x);

    relativeVelocity = v1 - v2;
    float relativeVelocityTangent = relativeVelocity.dot(tangent);

    // Calculate moment of inertia
    I1 = 0.05f * mainObject->getMass() * mainObject->getRadius() * mainObject->getRadius();
    I2 = 0.05f * otherObject->getMass() * otherObject->getRadius() * otherObject->getRadius();

    // Cross products for tangent
    float r1CrossT = r1.x * tangent.y - r1.y * tangent.x;
    float r2CrossT = r2.x * tangent.y - r2.y * tangent.x;

    // Friction impulse scala
    float frictionImpulseScalar = -relativeVelocityTangent * friction /
        ((1.0f / mainObject->getMass()) + (1.0f / otherObject->getMass()) +
        (r1CrossT * r1CrossT / I1) + (r2CrossT * r2CrossT / I2));

    sf::Vector2f frictionImpulse = tangent * frictionImpulseScalar;

    // Apply friction to linear velocity
    mainObject->setVelocity(mainObject->getVelocity() + frictionImpulse / mainObject->getMass());
    otherObject->setVelocity(otherObject->getVelocity() - frictionImpulse / otherObject->getMass());

    // Apply friction torque
    float frictionTorque1 = r1.x * frictionImpulse.y - r1.y * frictionImpulse.x;
    float frictionAngularImpulse1 = frictionTorque1 / I1;
    mainObject->setAngularVelocity(mainObject->getAngularVelocity() + frictionAngularImpulse1);
    float frictionTorque2 = -(r2.x * frictionImpulse.y - r2.y * frictionImpulse.x);
    float frictionAngularImpulse2 = frictionTorque2 / I2;
    otherObject->setAngularVelocity(otherObject->getAngularVelocity() + frictionAngularImpulse2);



    // Separate overlapping objects
    if (overlap > 0.0f) {
        // Mass-based separation (lighter object moves more)
        float totalMass = mainObject->getMass() + otherObject->getMass();
        float separation1 = overlap * (otherObject->getMass() / totalMass);
        float separation2 = overlap * (mainObject->getMass() / totalMass);

        mainObject->setPosition(mainObject->getPosition() - normal * separation1);
        otherObject->setPosition(otherObject->getPosition() + normal * separation2);
    }
}

void detectAndHandleHazards(std::shared_ptr<Player> player, 
                            std::vector<std::shared_ptr<Hazardous>>& hazardousObjects)
{
    for(auto& hazard : hazardousObjects)
    {
        bool overlap = detectIntersection(player->getPosition(), player->getRadius(), hazard->getPosition(), hazard->getRadius());
        if(overlap)
        {
            hazard->dealDamage(player);
        }
    }
}

void addDragForce(sf::Vector2f& currentVelocity, float mass, float deltaTime)
{
    currentVelocity.x = currentVelocity.x * (1.f - dragCoef * deltaTime / mass);
    currentVelocity.y = currentVelocity.y * (1.f - dragCoef * deltaTime / mass);
}

void addAccelerationForce(sf::Vector2f& currentVelocity, float acceleration, float direction, bool backward, float maximumVelocity, float mass, float deltaTime)
{
    // get the maximum components
    float maxXVelocity = maximumVelocity * std::abs(std::cos(direction));
    float maxYVelocity = maximumVelocity * std::abs(std::sin(direction));

    sf::Vector2f fAccel = {0, 0};
    if(acceleration != 0)
    {
        fAccel.x = (-acceleration / mass) * std::cos(direction) * deltaTime;
        fAccel.y = (-acceleration / mass) * std::sin(direction) * deltaTime;
        if(backward) // reverse direction
        {
            fAccel.x *= -1;
            fAccel.y *= -1;
        }
    }
    sf::Vector2f newVelocity = {currentVelocity.x + fAccel.x, currentVelocity.y + fAccel.y};

    // only add acceleration if we wouldn't go over the max
    if(std::abs(newVelocity.x) <= maxXVelocity)
        currentVelocity.x = newVelocity.x;
    if(std::abs(newVelocity.y) <= maxYVelocity)
        currentVelocity.y = newVelocity.y;
}

void updateButtonPresses()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
    {
        // std::cout << "Key Pressed: Escape" << std::endl;
        *buttons[0] = true;
    }else
{
        *buttons[0] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        // std::cout << "Key Pressed: W/Up" << std::endl;
        *buttons[1] = true;
    }else
{
        *buttons[1] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        // std::cout << "Key Pressed: S/Down" << std::endl;
        *buttons[2] = true;
    }else 
{
        *buttons[2] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        // std::cout << "Key Pressed: A/Left" << std::endl;
        *buttons[3] = true;
    }else
{
        *buttons[3] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        // std::cout << "Key Pressed: D/Right" << std::endl;
        *buttons[4] = true;
    }else
{
        *buttons[4] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        // std::cout << "E or Space Pressed" << std::endl;
        *buttons[5] = true;
    }else
{
        *buttons[5] = false;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab))
    {
        // std::cout << "E or Space Pressed" << std::endl;
        *buttons[6] = true;
    }else
{
        *buttons[6] = false;
    }
}

void updateGame(std::shared_ptr<Player> player, 
                bool* (&buttons)[numButtons], 
                std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
                std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects, 
                std::vector<std::shared_ptr<Interactable>>& interactableObjects, 
                std::vector<std::shared_ptr<Hazardous>>& hazardousObjects,
                QuadTree& quadTree)
{
    // get the buttons that the player is pushing first
    // handle actions as needed
    player->playerUpdate(buttons);
    // interact
    if(*buttons[5])
    {
        detectAndHandleInteractions(player, interactableObjects);
    }

    // setup the tree to handle collisions
    quadTree.clear();
    for(auto& obj : physicalObjects)
    quadTree.insert(obj);

    // pre-allocate reusable vector intead of creating a bunch every frame
    std::vector<std::shared_ptr<PhysicalObject>> nearbyCache;
    nearbyCache.reserve(500);
    auto playerPtr = std::dynamic_pointer_cast<Player>(player);
    for(auto& obj : physicalObjects)
    {
        obj->physicalUpdate();

        // don't handle collisions of far away objects
        sf::Vector2f objClosesetPosition = getClosestWrapPosition(player->getPosition(), obj->getPosition());
        float dx = objClosesetPosition.x - player->getPosition().x;
        float dy = objClosesetPosition.y - player->getPosition().y;
        if(std::sqrt(dx * dx + dy * dy) < (std::sqrt(viewWidth * viewWidth + viewHeight * viewHeight) / 2))
        {
            nearbyCache.clear();
            quadTree.retrieveToroidal(nearbyCache, obj->getPosition(), obj->getRadius());
            for(auto& other : nearbyCache)
            // for(auto& other : physicalObjects)
            {
                if(obj->getObjectID() == other->getObjectID()) continue;

                // accounts for closest position
                bool overlap = detectIntersection(obj->getPosition(), obj->getRadius(),
                                                  other->getPosition(), other->getRadius());

                if(overlap)
                {

                    bool objIsPlayer = (obj == player);
                    bool otherIsPlayer = (other == player);

                    if(objIsPlayer || otherIsPlayer)
                    {
                        handleCollision(obj, other, restitution, friction);
                    }
                    else
                {
                        // less intensive collision handling for non player
                        handleCollision(obj, other);
                    }

                    if(objIsPlayer)
                    {
                        auto hazard = std::dynamic_pointer_cast<Hazardous>(other);
                        if(hazard) hazard->dealDamage(playerPtr);
                    } else if(otherIsPlayer)
                    {
                        auto hazard = std::dynamic_pointer_cast<Hazardous>(obj);
                        if(hazard) hazard->dealDamage(playerPtr);
                    }
                }
            }
        }
    }

    // non-physical hazards
    detectAndHandleHazards(player, hazardousObjects);
    // update visual objects then UI
    for(auto& obj : visualObjects)
    {
        obj->basicUpdate(FixedDeltaTime, player->getVelocity());
    }
}
void updateUI(sf::RenderWindow& window, std::vector<std::unique_ptr<UIElement>>& UIElements)
{
    window.setView(window.getDefaultView());
    for(auto& element : UIElements)
    {
        element->update(window);
    }
}

void updateMinimap(sf::RenderWindow& window, Minimap& minimap)
{
    minimap.updateViewport();
    window.setView(*minimap.getView());
    minimap.update(window);
}

void drawGame(sf::RenderWindow& window, sf::View& view, std::shared_ptr<Player> player, 
              std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
              std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects)
{
    // Get view in correct spot
    view.setCenter(player->getPosition());
    window.setView(view);

    // draw visual objects
    for(auto& obj : visualObjects)
    {
        obj->basicDraw(window);
    }

    // draw physical objects
    for(auto& obj : physicalObjects)
    {
        obj->physicalDraw(window);
    }

    // draw the player
    player->physicalDraw(window);

}

void drawUI(sf::RenderWindow& window, std::vector<std::unique_ptr<UIElement>>& UIElements)
{
    window.setView(window.getDefaultView());
    for(auto& element : UIElements)
    {
        element->render(window);
    }
}

void drawMinimap(sf::RenderWindow& window, Minimap& minimap)
{
    window.setView(*minimap.getView());
    minimap.render(window);
}

void setupGame(std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
               std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects, 
               std::vector<std::shared_ptr<Interactable>>& interactableObjects, 
               std::vector<std::shared_ptr<Hazardous>>& hazardousObjects,
               const std::shared_ptr<Player>& player)
{
    // temporary
    hazardousObjects.clear();

    // player position (900, 500);

    // random distrubution for any objects to use
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> distX(0.f, (float)worldWidth);
    std::uniform_real_distribution<float> distY(0.f, (float)worldHeight);

    // VISUAL OBJECTS
    // far background
    sf::Vector2i bSize(1000 * worldWidth, 1000 * worldHeight);
    sf::Vector2f bPosition({0, 0});
    float bRotation = M_PI / 2;
    RenderLayer bRenderLayer = RenderLayer::FarBackground;
    RenderLayer bRenderLayer2 = RenderLayer::CloseBackground;
    VisualObject bg(bPosition, bSize, bRotation, bRenderLayer, &farBackgroundTexture);
    VisualObject fg(bPosition, bSize, bRotation, bRenderLayer2, &closeBackgroundTexture);
    visualObjects.push_back(std::make_unique<VisualObject>(bg));
    visualObjects.push_back(std::make_unique<VisualObject>(fg));



    // PHYSICAL OBJECTS

    // testing meteor
    float objectRadius = 8; // used for specific placement, not important
    // two objects VERY close to eachother on opposite sides of the world
    std::vector<sf::Vector2f*> mPositions[2];
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(0.1, 0.1))));
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(worldWidth - 2*objectRadius, 0.1))));

    // two objects touching eachother near the middle of the board
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(500, 200))));
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(500 + objectRadius, 200.5))));

    // two objects near above player start to test collision rotation
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(900, 400))));
    physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(914, 340))));

    for(int i = 0; i < 5; i++)
    {
        physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(distX(rng), distY(rng)))));
    }

    // crates
    auto c1 = std::make_shared<HealthCrate>(HealthCrate(sf::Vector2f(100, 100)));
    physicalObjects.push_back(c1);
    interactableObjects.push_back(c1);


    // Hazards
    // create hazards
    // auto h0 = std::make_shared<Spikey>(Spikey(sf::Vector2f(distX(rng), distY(rng))));
    // physicalObjects.push_back(h0);

    // a spikey on the four courners
    auto h1 = std::make_shared<Spikey>(Spikey({10, 10}));
    auto h3 = std::make_shared<Spikey>(Spikey({worldWidth - 10, 10}));
    auto h2 = std::make_shared<Spikey>(Spikey({10, worldHeight - 10}));
    auto h4 = std::make_shared<Spikey>(Spikey({worldWidth - 10, worldHeight - 10}));
    physicalObjects.push_back(h1);
    physicalObjects.push_back(h2);
    physicalObjects.push_back(h3);
    physicalObjects.push_back(h4);
    // no longer needed for physical hazards, will be needed for visual hazards (non-physical)
    // hazardousObjects.push_back(h1);


    // int numEnemies = 600; // approx max without lag (pre weapon addition)
    int numEnemies = 10;
    for(int i = 0; i < numEnemies; i++)
    {
        auto eR = std::make_shared<Enemy1>(Enemy1(sf::Vector2f(distX(rng), distY(rng)), player));
        physicalObjects.push_back(eR);

        // no longer needed for physical hazards, will be needed for visual hazards (non-physical)
        // hazardousObjects.push_back(eR);
    }
}

void setupUI(std::vector<std::unique_ptr<UIElement>>& UIElements, std::shared_ptr<Player>& player)
{
    // health bar
    UIElements.push_back(std::make_unique<UIHealth>(UIHealth(player)));
}

int main()
{
    initializeButtons();
    initializeTextures();

    // setup game objects
    // player
    std::shared_ptr<Player> player = std::make_shared<Player>(Player());

    std::vector<std::unique_ptr<VisualObject>> visualObjects;
    std::vector<std::shared_ptr<PhysicalObject>> physicalObjects;
    std::vector<std::shared_ptr<Interactable>> interactableObjects;
    std::vector<std::shared_ptr<Hazardous>> hazardousObjects;
    physicalObjects.push_back(player);
    setupGame(visualObjects, physicalObjects, interactableObjects, hazardousObjects, player);

    // UI
    std::vector<std::unique_ptr<UIElement>> UIElements;
    setupUI(UIElements, player);

    // window
    sf::Vector2<uint> winSize(windowWidth, windowHeight);
    sf::RenderWindow window(sf::VideoMode(winSize), "game");
    // window.setFramerateLimit(frameRateLimit);
    window.setVerticalSyncEnabled(true);
    window.setPosition(windowPos);

    // primary view
    sf::View playerView(player->getPosition() + player->getSprite().getOrigin(), {(float)viewWidth, (float)viewHeight});
    playerView.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
    window.setView(playerView);

    // minimap view
    std::shared_ptr<sf::View> minimapView = std::make_shared<sf::View>(sf::View(sf::Vector2f(0, 0), {worldWidth, worldHeight}));
    minimapView->setCenter({worldWidth / 2.f, worldHeight / 2.f});
    minimapView->setViewport(sf::FloatRect({0,0},{0,0}));
    auto minimap = std::make_unique<Minimap>(Minimap(player, minimapView));

    // time
    sf::Clock clock;
    float timeAccumulator = 0.0f;

    // spacial grid for more efficient collision detection
    QuadTree quadTree(0, sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(worldWidth, worldHeight)));

    while(window.isOpen())
    {
        float frameTime = clock.restart().asSeconds();
        if(frameTime > 0.25)
            frameTime = 0.25;
        timeAccumulator += frameTime;

        updateButtonPresses();
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>() || escPressed)
            {
                window.close();
            }else if(const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // Update the view to the new size of the window to prevent stretching
                windowWidth = resized->size.x;
                windowHeight = resized->size.y;
                viewWidth = windowWidth;
                viewHeight = windowHeight;
                playerView.setSize({(float)windowWidth, (float)windowHeight});
                // window.setView(playerView); // automatically updates with the view since it's a reference

                // std::cout << "New window width: " << resized->size.x << std::endl;
                // std::cout << "New window height: " << resized->size.y << std::endl;
            }
        }

        int maxUpdates = 3;
        int updateCount = 0;
        while(timeAccumulator >= FixedDeltaTime && updateCount < maxUpdates)
        {
            updateGame(player, buttons, visualObjects, physicalObjects, interactableObjects, hazardousObjects, quadTree);
            updateUI(window, UIElements);
            updateMinimap(window, *minimap);

            timeAccumulator -= FixedDeltaTime;
            updateCount++;
        }
        if(updateCount >= maxUpdates)
        {
            timeAccumulator = 0;
        }

        // clear the window, then draw
        window.clear();
        drawGame(window, playerView, player, visualObjects, physicalObjects);
        drawUI(window, UIElements);
        drawMinimap(window, *minimap);
        window.display();
    }
    return 0;
}
