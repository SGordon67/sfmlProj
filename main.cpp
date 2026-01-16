#include "HealthCrate.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <random>

#include "globals.h"
#include "enums.h"
#include "BasicObject.h"
#include "VisualObject.h"
#include "Crate.h"
#include "PhysicalObject.h"
#include "Ball.h"
#include "Player.h"
#include "Spikey.h"

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

void detectAndHandleInteractions(Player& player, std::vector<std::shared_ptr<Interactable>>& interactableObjects)
{
	for(auto& obj : interactableObjects)
	{
		bool overlap = detectIntersection(player.getPosition(), player.getRadius(), obj->getPosition(), obj->getInteractionRadius());
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

// void handleCollision(PhysicalObject& mainObject, PhysicalObject& otherObject, float restitution)
// {
void handleCollision(PhysicalObject& mainObject, PhysicalObject& otherObject,
                     float restitution = 1.0f, float friction = 0.5f)
{
	std::cout << "COLLISION BETWEEN OBJECTS: " << mainObject.getObjectID() << ", and " << otherObject.getObjectID() << ". " << std::endl;
	std::cout << "\tCollision occurred at position: (" << mainObject.getPosition().x << ", " << mainObject.getPosition().y 
        << ") and (" << otherObject.getPosition().x << ", " << otherObject.getPosition().y << ")" << std::endl;

	// varials to keep track of collision information
	sf::Vector2f normal;
	sf::Vector2f contact1;
	sf::Vector2f contact2;
	sf::Vector2f r1;
	sf::Vector2f r2;
	float overlap;

	// Calculate the collision normal
	sf::Vector2f delta = otherObject.getPosition() - mainObject.getPosition();
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
	contact1 = mainObject.getPosition() + normal * mainObject.getRadius();
	contact2 = otherObject.getPosition() - normal * otherObject.getRadius();
	// Radii from center of mass to contact points
	r1 = contact1 - mainObject.getPosition();
	r2 = contact2 - otherObject.getPosition();
	// Calculate overlap
	overlap = (mainObject.getRadius() + otherObject.getRadius()) - distance;



	// Apply normal impulse (velocity response)
	// Calculate velocity at contact points (including rotational velocity)
	sf::Vector2f v1 = mainObject.getVelocity() + 
	                  sf::Vector2f(-mainObject.getAngularVelocity() * r1.y,
	                               mainObject.getAngularVelocity() * r1.x);
	sf::Vector2f v2 = otherObject.getVelocity() + 
	                  sf::Vector2f(-otherObject.getAngularVelocity() * r2.y,
	                               otherObject.getAngularVelocity() * r2.x);
	// Relative velocity
	sf::Vector2f relativeVelocity = v1 - v2;
	// Project velocity onto normal
	float velocityAlongNormal = relativeVelocity.dot(normal);
	// Objects already separating, don't apply impulse
	if (velocityAlongNormal < 0.0f) return;
	// Calculate moment of inertia
	float I1 = 0.05f * mainObject.getMass() * mainObject.getRadius() * mainObject.getRadius();
	float I2 = 0.05f * otherObject.getMass() * otherObject.getRadius() * otherObject.getRadius();
	// Cross products for rotational contributio
	float r1CrossN = r1.x * normal.y - r1.y * normal.x;
	float r2CrossN = r2.x * normal.y - r2.y * normal.x;
	// Calculate impulse scalar
	float impulseScalar = (-(1.0f + restitution) * velocityAlongNormal) /
	                      ((1.0f / mainObject.getMass()) + (1.0f / otherObject.getMass()) +
                           (r1CrossN * r1CrossN / I1) + (r2CrossN * r2CrossN / I2));
	// Apply impulse to linear velocitie
	sf::Vector2f impulse = normal * impulseScalar;
	mainObject.setVelocity(mainObject.getVelocity() + impulse / mainObject.getMass());
	otherObject.setVelocity(otherObject.getVelocity() - impulse / otherObject.getMass());
	// Apply rotational impulse
	float torque1 = r1.x * impulse.y - r1.y * impulse.x;
	float angularImpulse1 = torque1 / I1;
	mainObject.setAngularVelocity(mainObject.getAngularVelocity() + angularImpulse1);
	float torque2 = r2.x * impulse.y - r2.y * impulse.x;
	float angularImpulse2 = torque2 / I2;
	otherObject.setAngularVelocity(otherObject.getAngularVelocity() + angularImpulse2);



	// Apply friction impulse (creates rotation for circles)
	// Calculate tangent (perpendicular to normal)
	sf::Vector2f tangent(-normal.y, normal.x);
	
	// Velocity at contact points
	v1 = mainObject.getVelocity() +
	                  sf::Vector2f(-mainObject.getAngularVelocity() * r1.y,
	                               mainObject.getAngularVelocity() * r1.x);
	v2 = otherObject.getVelocity() +
	                  sf::Vector2f(-otherObject.getAngularVelocity() * r2.y,
	                               otherObject.getAngularVelocity() * r2.x);

	relativeVelocity = v1 - v2;
	float relativeVelocityTangent = relativeVelocity.dot(tangent);

	// Calculate moment of inertia
	I1 = 0.05f * mainObject.getMass() * mainObject.getRadius() * mainObject.getRadius();
	I2 = 0.05f * otherObject.getMass() * otherObject.getRadius() * otherObject.getRadius();
	
	// Cross products for tangent
	float r1CrossT = r1.x * tangent.y - r1.y * tangent.x;
	float r2CrossT = r2.x * tangent.y - r2.y * tangent.x;
	
	// Friction impulse scala
	float frictionImpulseScalar = -relativeVelocityTangent * friction /
	                              ((1.0f / mainObject.getMass()) + (1.0f / otherObject.getMass()) +
	                               (r1CrossT * r1CrossT / I1) + (r2CrossT * r2CrossT / I2));
	
	sf::Vector2f frictionImpulse = tangent * frictionImpulseScalar;

	// Apply friction to linear velocity
	mainObject.setVelocity(mainObject.getVelocity() + frictionImpulse / mainObject.getMass());
	otherObject.setVelocity(otherObject.getVelocity() - frictionImpulse / otherObject.getMass());
	
	// Apply friction torque
	float frictionTorque1 = r1.x * frictionImpulse.y - r1.y * frictionImpulse.x;
	float frictionAngularImpulse1 = frictionTorque1 / I1;
	mainObject.setAngularVelocity(mainObject.getAngularVelocity() + frictionAngularImpulse1);

	float frictionTorque2 = -(r2.x * frictionImpulse.y - r2.y * frictionImpulse.x);
	float frictionAngularImpulse2 = frictionTorque2 / I2;
	otherObject.setAngularVelocity(otherObject.getAngularVelocity() + frictionAngularImpulse2);



	// Separate overlapping objects
	if (overlap > 0.0f) {
		std::cout << "Separating overlapping hitboxes with overlap: " << overlap << std::endl;
		// Mass-based separation (lighter object moves more)
		float totalMass = mainObject.getMass() + otherObject.getMass();
		float separation1 = overlap * (otherObject.getMass() / totalMass);
		float separation2 = overlap * (mainObject.getMass() / totalMass);

		mainObject.setPosition(mainObject.getPosition() - normal * separation1);
		otherObject.setPosition(otherObject.getPosition() + normal * separation2);
	}
}

void detectAndHandleHazards(Player& player, 
    std::vector<std::shared_ptr<Hazardous>>& hazardousObjects)
{
    for(auto& hazard : hazardousObjects)
    {
		bool overlap = detectIntersection(player.getPosition(), player.getRadius(), hazard->getPosition(), hazard->getRadius());
        if(overlap)
        {
            std::cout << "OVERLAP WITH A HAZARD" << std::endl;
            hazard->dealDamage(player);
        }
    }
}

void detectAndHandleCollision(PhysicalObject& mainObject, 
        std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects)
{
	for(auto& obj : physicalObjects)
	{
		if(mainObject.getObjectID() == obj->getObjectID()) continue;
		bool overlap = detectIntersection(mainObject.getPosition(), mainObject.getRadius(), obj->getPosition(), obj->getRadius());
		if(overlap)
		{
			handleCollision(mainObject, *obj, restitution, friction);
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
	
	// std::cout << "Maximum velocity allowed: " << maximumVelocity << ", (" << maxXVelocity << ", " << maxYVelocity << ")" << std::endl;
	// std::cout << "Current velocity " << currentVelocity.x << ", " << currentVelocity.y << std::endl;

	sf::Vector2f fAccel = {0, 0};
	if(acceleration != 0)
	{
		if(acceleration > 0)
		{
			fAccel.x = (-acceleration / mass) * std::cos(direction) * deltaTime;
			fAccel.y = (-acceleration / mass) * std::sin(direction) * deltaTime;
		} else if(backward) // reverse direction
		{
			fAccel.x = (0.5f * acceleration / mass) * std::cos(direction) * deltaTime;
			fAccel.y = (0.5f * acceleration / mass) * std::sin(direction) * deltaTime;
		}
	}
	sf::Vector2f newVelocity = {currentVelocity.x + fAccel.x, currentVelocity.y + fAccel.y};

	// only add acceleration if we wouldn't go over the max
	if(std::abs(newVelocity.x) <= maxXVelocity)
	{
		// std::cout << "Max not reached in x direction, accelerate" << std::endl;
		currentVelocity.x = newVelocity.x;
	}
	// else std::cout << "Max x velocity reached, acceleration ignored" << std::endl;
	if(std::abs(newVelocity.y) <= maxYVelocity)
	{
		// std::cout << "Max not reached in y direction, accelerate" << std::endl;
		currentVelocity.y = newVelocity.y;
	}
	// else std::cout << "Max y velocity reached, acceleration ignored" << std::endl;

	// std::cout << "Acceleration Force: " << fAccel.x << ", " << fAccel.y << std::endl;
}

void updateButtonPresses(bool* (&buttons)[numButtons])
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

void updateGame(Player& player, 
        bool* (&buttons)[numButtons], 
        std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
        std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects, 
        std::vector<std::shared_ptr<Interactable>>& interactableObjects, 
        std::vector<std::shared_ptr<Hazardous>>& hazardousObjects)
{
	// update visual objects
	for(auto& obj : visualObjects)
	{
		obj->basicUpdate(FixedDeltaTime, player.getVelocity());
	}
	
	// update physical objects
	for(auto& obj : physicalObjects)
	{
		obj->physicalUpdate();
		detectAndHandleCollision(*obj, physicalObjects);
	}

	player.playerUpdate(buttons);
	if(*buttons[5])
	{
		detectAndHandleInteractions(player, interactableObjects);
	}
	detectAndHandleHazards(player, hazardousObjects);
	detectAndHandleCollision(player, physicalObjects);
}

void drawGame(sf::RenderWindow& window, sf::View& view, Player& player, std::vector<std::unique_ptr<VisualObject>>& visualObjects, std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects)
{
	// clear the window, then draw, then display
	window.clear();

	// Get view in correct spot
	view.setCenter(player.getPosition());
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
	player.physicalDraw(window);
	window.display();
}

void setupGame(std::vector<std::unique_ptr<VisualObject>>& visualObjects, 
        std::vector<std::shared_ptr<PhysicalObject>>& physicalObjects, 
        std::vector<std::shared_ptr<Interactable>>& interactableObjects, 
        std::vector<std::shared_ptr<Hazardous>>& hazardousObjects)
{
	// VISUAL OBJECTS
	// far background
	sf::Vector2i bSize(1000 * windowWidth, 1000 * windowHeight);
	sf::Vector2f bPosition({0, 0});
	float bRotation = M_PI / 2;
	RenderLayer bRenderLayer = RenderLayer::FarBackground;
	RenderLayer bRenderLayer2 = RenderLayer::CloseBackground;
	std::string bgtFilename = "art/basicBackground.png";
	std::string bstFilename = "art/basicStars.png";
	VisualObject bg(bPosition, bSize, bRotation, bRenderLayer, bgtFilename);
	VisualObject fg(bPosition, bSize, bRotation, bRenderLayer2, bstFilename);

	visualObjects.push_back(std::make_unique<VisualObject>(bg));
	visualObjects.push_back(std::make_unique<VisualObject>(fg));



	// PHYSICAL OBJECTS

	// testing meteor
	sf::Vector2i objectSize(17, 17);
	float objectRotation = M_PI / 2;
	RenderLayer objectRenderLayer = RenderLayer::Main;
	std::string objectFilename = "art/basicMeteor.png";
	float objectMass = 10;
	float objectRadius = 8;
	sf::Vector2f objectVelocity = {0, 0};
	float objectAcceleration = 0;
	float objectAngularVelocity = 0;
	float objectMaximumVelocity = 500; // equal to players for now

	// custom objects
	// player position (900, 500);
	
	// two objects VERY close to eachother on opposite sides of the world
	std::vector<sf::Vector2f*> mPositions[2];
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(0.1, 0.1), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(worldWidth - 2*objectRadius, 0.1), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));

	// two objects touching eachother near the middle of the board
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(500, 200), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(500 + objectRadius, 200.5), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));

	// two objects near above player start to test collision rotation
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f({900, 400}), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));
	physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f({914, 340}), objectSize, objectRotation, objectRenderLayer, objectFilename, 
				       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));
	
	// create 10 objects randomely on the first screen
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> distX(0.f, (float)windowWidth);
	std::uniform_real_distribution<float> distY(0.f, (float)windowHeight);
	for(int i = 0; i < 5; i++)
	{
		// random objects
		physicalObjects.push_back(std::make_shared<Ball>(Ball(sf::Vector2f(distX(rng), distY(rng)), objectSize, objectRotation, objectRenderLayer, objectFilename, 
					       objectMass, objectRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity)));
	}

	// crates
	sf::Vector2f cPosition({100, 100});
	sf::Vector2i cSize(17, 17);
	float cRotation = M_PI / 2;
	RenderLayer cRenderLayer = RenderLayer::Main;
	std::string cFilename = "art/basicCrate.png";
	float cMass = 10;
	float cRadius = 8;
	sf::Vector2f cVelocity = {0, 0};
	float cAcceleration = 0;
	float cAngularVelocity = 0;
	float cMaximumVelocity = 500; // equal to players for now
    int cHP = 100;
    int cMaxHP = 100;
    int cValue = 10;
    
    auto c1 = std::make_shared<HealthCrate>(HealthCrate(cPosition, cSize, cRotation, cRenderLayer, cFilename, 
                    cMass, cRadius, cVelocity, cAcceleration, cAngularVelocity, cMaximumVelocity, cHP, cMaxHP, cValue));
	physicalObjects.push_back(c1);
	interactableObjects.push_back(c1);


    // Hazards
	sf::Vector2i spikeySize(20, 20);
    int spikeyHP = 100;
    int spikeyMaxHP = 100;
    int spikeyDamage = 10;
	std::string spikeyFilename = "art/basicSpikey.png";
	float spikeyRadius = 10;

	// create hazards
    auto h1 = std::make_shared<Spikey>(sf::Vector2f(distX(rng), distY(rng)), spikeySize, objectRotation, objectRenderLayer, spikeyFilename, 
                objectMass, spikeyRadius, objectVelocity, objectAcceleration, objectAngularVelocity, objectMaximumVelocity, spikeyHP, spikeyMaxHP, spikeyDamage);
    physicalObjects.push_back(h1);
	hazardousObjects.push_back(h1);
}

int main()
{
	// keys
	bool escPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool interactPressed = false;
	bool tabPressed = false;
	bool* buttons[numButtons];
	buttons[0] = &escPressed;
	buttons[1] = &upPressed;
	buttons[2] = &downPressed;
	buttons[3] = &leftPressed;
	buttons[4] = &rightPressed;
	buttons[5] = &interactPressed;
	buttons[6] = &tabPressed;

	// setup game objects
	
	// player
	sf::Vector2f pPosition(900, 500);
	sf::Vector2i pSize(24, 30);
	float pRotation = M_PI / 2;
	RenderLayer pRenderLayer = RenderLayer::Main;
	std::string pFilename = "art/basicSpriteL.png";
	float pMass = 10;
	float pRadius = pSize.x / 2.f;
	sf::Vector2f pVelocity = {0, 0};
	float pAcceleration = 5000;
	float pAngularVelocity = 0;
	float pMaxVelocity = 500;
	int pMaxHP = 100;
	float angularAccleration = degreesToRadians(4);
	Player player = Player(pPosition, pSize, pRotation, pRenderLayer, pFilename, 
            pMass, pRadius, pVelocity, pAcceleration, pAngularVelocity, pMaxVelocity, 
            pMaxHP, pMaxHP, angularAccleration);



	std::vector<std::unique_ptr<VisualObject>> visualObjects;
	std::vector<std::shared_ptr<PhysicalObject>> physicalObjects;
	std::vector<std::shared_ptr<Interactable>> interactableObjects;
    std::vector<std::shared_ptr<Hazardous>> hazardousObjects;
	setupGame(visualObjects, physicalObjects, interactableObjects, hazardousObjects);




	// window
	sf::Vector2<uint> winSize(windowWidth, windowHeight);
	sf::RenderWindow window(sf::VideoMode(winSize), "game");
	window.setFramerateLimit(frameRateLimit);
	window.setPosition(windowPos);

	//view
	sf::View playerView(player.getPosition() + player.getSprite().getOrigin(), {(float)viewWidth, (float)viewHeight});
	window.setView(playerView);

	// time
	sf::Clock clock;
	float timeAccumulator = 0.0f;

	while(window.isOpen())
	{
		float frameTime = clock.restart().asSeconds();
		if(frameTime > 0.25)
			frameTime = 0.25;
		timeAccumulator += frameTime;

		updateButtonPresses(buttons);
		while(const std::optional event = window.pollEvent())
		{
			if(event->is<sf::Event::Closed>() || escPressed)
			{
				window.close();
			}
		}

		while(timeAccumulator >= FixedDeltaTime)
		{
			updateGame(player, buttons, visualObjects, physicalObjects, interactableObjects, hazardousObjects);
			timeAccumulator -= FixedDeltaTime;
		}
		drawGame(window, playerView, player, visualObjects, physicalObjects);
	}
	return 0;
}
