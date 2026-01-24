#include "HealthCrate.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <chrono>
#include <iostream>
#include <cmath>
#include <memory>
#include <random>
#include <ratio>
#include <vector>

#include "SpatialHashGrid.h"
#include "UIElement.h"
#include "UIHealth.h"
#include "globals.h"
#include "enums.h"
#include "BasicObject.h"
#include "VisualObject.h"
#include "Crate.h"
#include "PhysicalObject.h"
#include "Ball.h"
#include "Player.h"
#include "Spikey.h"
#include "Enemy.h"
#include "Enemy1.h"

float degreesToRadians(float degrees)
{
	return (degrees * (M_PI / 180));
}

float radiansToDegrees(float radians)
{
	return (radians * (180 / M_PI));
}

sf::Vector2f wrapPosition(sf::Vector2f position)
{
	position.x = std::fmod(position.x, worldWidth);
	position.y = std::fmod(position.y, worldHeight);

	if(position.x < 0) position.x += worldWidth;
	if(position.y < 0) position.y += worldHeight;

	return position;
}

bool detectIntersection(sf::Vector2f pos1, float radius1, sf::Vector2f pos2, float radius2)
{
	// shortest distance with wrapping
	sf::Vector2f delta = pos1 - pos2;
	if(std::abs(delta.x) > worldWidth / 2.0f)
	{
		delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
	}
	if(std::abs(delta.y) > worldHeight / 2.0f)
	{
		delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
	}

	float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	if(distance <= (radius1 + radius2))
	{
		return true;
	}
	return false;
}

void detectAndHandleInteractions(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Interactable>>& interactableObjects)
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

std::vector<sf::Vector2f> getDupPositions(sf::Vector2f position, sf::Vector2i size)
{
	std::vector<sf::Vector2f> dupPositions = {};

	float wrapXThreshold = (std::abs(worldWidth - (viewWidth / 2.f)) + size.x);
	float wrapYThreshold = (std::abs(worldHeight - (viewHeight / 2.f)) + size.y);

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

void handleCollision(std::shared_ptr<PhysicalObject>& mainObject, std::shared_ptr<PhysicalObject>& otherObject,
                     float restitution = 1.0f, float friction = 0.5f)
{
	// std::cout << "COLLISION -- " << mainObject.getObjectID() << " | " << otherObject.getObjectID() << std::endl;
	// std::cout << "\tCollision occurred at position: (" << mainObject.getPosition().x << ", " << mainObject.getPosition().y 
	//        << ") and (" << otherObject.getPosition().x << ", " << otherObject.getPosition().y << ")" << std::endl;

	// varials to keep track of collision information
	sf::Vector2f normal;
	sf::Vector2f contact1;
	sf::Vector2f contact2;
	sf::Vector2f r1;
	sf::Vector2f r2;
	float overlap;

	// Calculate the collision normal
	sf::Vector2f delta = otherObject->getPosition() - mainObject->getPosition();
	// Use the shortest path (accounting for position wrap)
	if (std::abs(delta.x) > worldWidth / 2.0f) {
		delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
	}
	if (std::abs(delta.y) > worldHeight / 2.0f) {
		delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
	}
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
	contact2 = otherObject->getPosition() - normal * otherObject->getRadius();
	// Radii from center of mass to contact points
	r1 = contact1 - mainObject->getPosition();
	r2 = contact2 - otherObject->getPosition();
	// Calculate overlap
	overlap = (mainObject->getRadius() + otherObject->getRadius()) - distance;



	// Apply normal impulse (velocity response)
	// Calculate velocity at contact points (including rotational velocity)
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
	// Apply rotational impulse
	float torque1 = r1.x * impulse.y - r1.y * impulse.x;
	float angularImpulse1 = torque1 / I1;
	mainObject->setAngularVelocity(mainObject->getAngularVelocity() + angularImpulse1);
	float torque2 = r2.x * impulse.y - r2.y * impulse.x;
	float angularImpulse2 = torque2 / I2;
	otherObject->setAngularVelocity(otherObject->getAngularVelocity() + angularImpulse2);



	// Apply friction impulse (creates rotation for circles)
	// Calculate tangent (perpendicular to normal)
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
		// std::cout << "Separating overlapping hitboxes with overlap: " << overlap << std::endl;
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

// void detectAndHandleCollision(std::shared_ptr<PhysicalObject>& mainObject, 
//         SpatialHashGrid& spatialGrid)
// {
//
//     std::vector<std::shared_ptr<PhysicalObject>> nearbyObjects = spatialGrid.getNearby(mainObject);
//
// 	for(auto& obj : nearbyObjects)
// 	{
// 		if(mainObject->getObjectID() == obj->getObjectID()) continue;
// 		bool overlap = detectIntersection(mainObject->getPosition(), mainObject->getRadius(), obj->getPosition(), obj->getRadius());
// 		if(overlap)
// 		{
// 			handleCollision(mainObject, obj, restitution, friction);
//
//             auto player1 = std::dynamic_pointer_cast<Player>(mainObject);
//             auto player2 = std::dynamic_pointer_cast<Player>(obj);
//
//             auto hazard1 = std::dynamic_pointer_cast<Hazardous>(mainObject);
//             auto hazard2 = std::dynamic_pointer_cast<Hazardous>(obj);
//
//             if(player1 && hazard2)
//             {
//                 hazard2->dealDamage(player1);
//             }else if(player2 && hazard1)
//             {
//                 hazard1->dealDamage(player2);
//             }
// 		}
// 	}
// }

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
        std::vector<std::unique_ptr<UIElement>>& UIElements,
        SpatialHashGrid& spatialGrid)
{
    auto totalStart = std::chrono::high_resolution_clock::now();

    auto interactionStart = std::chrono::high_resolution_clock::now();
	player->playerUpdate(buttons);
	if(*buttons[5])
	{
		detectAndHandleInteractions(player, interactableObjects);
	}
    auto interactionEnd = std::chrono::high_resolution_clock::now();
    float interactionTime = std::chrono::duration_cast<std::chrono::microseconds>(interactionEnd - interactionStart).count() / 1000.0f;

    auto gridStart = std::chrono::high_resolution_clock::now();
    spatialGrid.clear();
    for(auto& obj : physicalObjects)
        spatialGrid.insert(obj);
    auto gridEnd = std::chrono::high_resolution_clock::now();
    float gridTime = std::chrono::duration_cast<std::chrono::microseconds>(gridEnd - gridStart).count() / 1000.0f;

    // pre-allocate reusable vector intead of creatin a bunch every frame
    auto physicsStart = std::chrono::high_resolution_clock::now();
    int collisionChecks = 0;
    int actualCollisions = 0;
    float totalUpdateTime = 0;
    float totalGridQueryTime = 0;
    float totalCollisionCheckTime = 0;

    std::vector<std::shared_ptr<PhysicalObject>> nearbyCache;
    nearbyCache.reserve(50);
    auto playerPtr = std::dynamic_pointer_cast<Player>(player);
    for(auto& obj : physicalObjects)
    {
        auto updateStart = std::chrono::high_resolution_clock::now();
        obj->physicalUpdate();
        auto updateEnd = std::chrono::high_resolution_clock::now();
        totalUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(updateEnd - updateStart).count() / 1000.0f;

        // don't handle collisions of far away objects
        float dx = obj->getPosition().x - player->getPosition().x;
        float dy = obj->getPosition().y - player->getPosition().y;
        float distSqr = dx * dx + dy * dy;
        if(distSqr < 4000000.0f)
        {
            auto gridQueryStart = std::chrono::high_resolution_clock::now();
            nearbyCache.clear();
            spatialGrid.getNearby(obj, nearbyCache);
            auto gridQueryEnd = std::chrono::high_resolution_clock::now();
            totalGridQueryTime += std::chrono::duration_cast<std::chrono::microseconds>(gridQueryEnd - gridQueryStart).count() / 1000.0f;

            for(auto& other : nearbyCache)
            {
                if(obj->getObjectID() == other->getObjectID()) continue;

                collisionChecks++;

                auto collisionStart = std::chrono::high_resolution_clock::now();
                bool overlap = detectIntersection(obj->getPosition(), obj->getRadius(),
                        other->getPosition(), other->getRadius());

                if(overlap)
                {
                    handleCollision(obj, other, restitution, friction);

                    bool objIsPlayer = (obj == player);
                    bool otherIsPlayer = (other == player);

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
                auto collisionEnd = std::chrono::high_resolution_clock::now();
                totalCollisionCheckTime += std::chrono::duration_cast<std::chrono::microseconds>(collisionEnd - collisionStart).count() / 1000.0f;
            }
        }
    }
    auto physicsEnd = std::chrono::high_resolution_clock::now();
    float physicsTime = std::chrono::duration_cast<std::chrono::microseconds>(physicsEnd - physicsStart).count() / 1000.0f;


    auto otherStart = std::chrono::high_resolution_clock::now();
    // non-physical hazards
	detectAndHandleHazards(player, hazardousObjects);
	// update visual objects then UI
	for(auto& obj : visualObjects)
	{
		obj->basicUpdate(FixedDeltaTime, player->getVelocity());
	}
    for(auto& element : UIElements)
    {
        element->update();
    }
    auto otherEnd = std::chrono::high_resolution_clock::now();
    float otherTime = std::chrono::duration_cast<std::chrono::microseconds>(otherEnd - otherStart).count() / 1000.0f;


    auto totalEnd = std::chrono::high_resolution_clock::now();
    float totalTime = std::chrono::duration_cast<std::chrono::microseconds>(totalEnd - totalStart).count() / 1000.0f;
    if(totalTime > 20.0f) {
        std::cout << "UPDATE BREAKDOWN (" << totalTime << "ms):" << std::endl;
        std::cout << "  Grid: " << gridTime << "ms" << std::endl;
        std::cout << "  Physics: " << physicsTime << "ms" << std::endl;
        std::cout << "    - physicalUpdate(): " << totalUpdateTime << "ms" << std::endl;
        std::cout << "    - Grid queries: " << totalGridQueryTime << "ms" << std::endl;
        std::cout << "    - Collision checks: " << totalCollisionCheckTime << "ms" << std::endl;
        std::cout << "    - Checks performed: " << collisionChecks << std::endl;
        std::cout << "    - Actual collisions: " << actualCollisions << std::endl;
        std::cout << "  Other: " << otherTime << "ms" << std::endl;
    }
}

void drawGame(sf::RenderWindow& window, sf::View& view, std::shared_ptr<Player> player, 
        std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
        std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects,
        std::vector<std::unique_ptr<UIElement>>& UIElements)
{
	// clear the window, then draw, then display
	window.clear();
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

    // update the UI
    window.setView(window.getDefaultView());
    for(auto& element : UIElements)
    {
        element->render(window);
    }

	window.display();
}

void setupGame(std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
        std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects, 
        std::vector<std::shared_ptr<Interactable>>& interactableObjects, 
        std::vector<std::shared_ptr<Hazardous>>& hazardousObjects,
        std::shared_ptr<Player>& player)
{
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
    auto h1 = std::make_shared<Spikey>(Spikey(sf::Vector2f(distX(rng), distY(rng))));
    physicalObjects.push_back(h1);

    // no longer needed for physical hazards, will be needed for visual hazards (non-physical)
	// hazardousObjects.push_back(h1);


    int numEnemies = 500;
	for(int i = 0; i < numEnemies; i++)
	{
        auto eR = std::make_shared<Enemy1>(Enemy1(sf::Vector2f(distX(rng), distY(rng)), player));
        physicalObjects.push_back(eR);

        // no longer needed for physical hazards, will be needed for visual hazards (non-physical)
        // hazardousObjects.push_back(eR);
	}
}

void setupUI(std::vector<std::unique_ptr<UIElement>>& UIElements, std::shared_ptr<Player> player)
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
	window.setView(playerView);

	// time
	sf::Clock clock;
	float timeAccumulator = 0.0f;

    // spacial grid for more efficient collision detection
    SpatialHashGrid spacialGrid(100.0f);


    // timekeeping for debugging
    // static int frameCount = 0;
    static float totalUpdateTime = 0;
    static float totalDrawTime = 0;
    static float maxUpdateTime = 0;
    static float maxDrawTime = 0;

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
                playerView.setSize({(float)resized->size.x, (float)resized->size.y});

                // std::cout << "New window width: " << resized->size.x << std::endl;
                // std::cout << "New window height: " << resized->size.y << std::endl;
            }
		}

        // start timing update
        auto updateStart = std::chrono::high_resolution_clock::now();

        int maxUpdates = 3;
        int updateCount = 0;
		while(timeAccumulator >= FixedDeltaTime && updateCount < maxUpdates)
		{
			updateGame(player, buttons, visualObjects, physicalObjects, interactableObjects, hazardousObjects, UIElements, spacialGrid);
			timeAccumulator -= FixedDeltaTime;
            updateCount++;
		}
        if(updateCount >= maxUpdates)
        {
            timeAccumulator = 0;
        }

        auto updateEnd = std::chrono::high_resolution_clock::now();
        auto updateDuration = std::chrono::duration_cast<std::chrono::microseconds>(updateEnd - updateStart);
        float currentUpdate = updateDuration.count() / 1000.0f;
        totalUpdateTime += currentUpdate;
        if(currentUpdate > maxUpdateTime) maxUpdateTime = currentUpdate;
        if(currentUpdate > 10.0f)
        {
            // std::cout << "!!! UPDATE SPIKE: " << currentUpdate << "ms !!!" << std::endl;
        }


        // start timing draw
        auto drawStart = std::chrono::high_resolution_clock::now();

		drawGame(window, playerView, player, visualObjects, physicalObjects, UIElements);

        auto drawEnd = std::chrono::high_resolution_clock::now();
        auto drawDuration = std::chrono::duration_cast<std::chrono::microseconds>(drawEnd - drawStart);
        float currentDraw = drawDuration.count() / 1000.0f;
        totalDrawTime += currentDraw;
        if(currentDraw > maxDrawTime) maxDrawTime = currentDraw;
        if(currentDraw > 10.0f)
        {
            // std::cout << "!!! DRAW SPIKE: " << currentDraw << "ms !!!" << std::endl;
        }

        // frameCount++;
        // if(frameCount % 60)
        // {
        //     std::cout << "Avg Update: " << totalUpdateTime / 60.0f << "ms, "
        //         << "Avg Draw: " << totalDrawTime / 60.0f << "ms, "
        //         << "Total: " << (totalUpdateTime + totalDrawTime) / 60.0f << "ms" << std::endl;
        //     totalUpdateTime = 0;
        //     totalDrawTime = 0;
        // }
	}
	return 0;
}
