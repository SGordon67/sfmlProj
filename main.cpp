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
#include <random>

#include "globals.h"
#include "enums.h"
#include "BasicObject.h"
#include "VisualObject.h"
#include "StaticPhysicalObject.h"
#include "PhysicalObject.h"
#include "Player.h"

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

void handleCollision(PhysicalObject& mainObject, PhysicalObject& otherObject, float deltaTime, float restitution)
{
	std::cout << "COLLISION BETWEEN OBJECTS: " << mainObject.getObjectID() << ", and " << otherObject.getObjectID() << ". " << std::endl;
	std::cout << "\tCollision occurred at position: (" << mainObject.getPosition().x << ", " << mainObject.getPosition().y << ") and (" << otherObject.getPosition().x << ", " << otherObject.getPosition().y << ")" << std::endl;

	// calculate the collision normal
	sf::Vector2f delta = otherObject.getPosition() - mainObject.getPosition();

	// use the shortest path (accounting for the position wrap)
	if (std::abs(delta.x) > worldWidth / 2.0f) {
		delta.x = delta.x > 0 ? delta.x - worldWidth 
		                      : delta.x + worldWidth;
	}
	if (std::abs(delta.y) > worldHeight / 2.0f) {
		delta.y = delta.y > 0 ? delta.y - worldHeight 
		                      : delta.y + worldHeight;
	}

	float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

	// avoid 0 edge case
	if(distance == 0.0f)
	{
		delta = sf::Vector2f(1.0f, 0.0f);
		distance = 1.0f;
	}

	// normalize collision normal
	sf::Vector2f normal = delta / distance;

	// relative velocity
	sf::Vector2f relativeVelocity = mainObject.getVelocity() - otherObject.getVelocity();

	// project the velocity to the normal
	float velocityAlongNormal = relativeVelocity.dot(normal);

	// prevent collision being reversed if objects are still touching and moveing away from eachother
	if(velocityAlongNormal < 0.0f) return;

	// calculate impulse scalar (restitution = 1 means perfectly elastic collision)
	float impulseScalar = (-(1.0f + restitution) * velocityAlongNormal) / 
			((1.0f / mainObject.getMass()) + (1.0f / otherObject.getMass()));

	// apply impulse to velocities
	sf::Vector2f impulse = normal * impulseScalar;
	mainObject.setVelocity(mainObject.getVelocity() + impulse / mainObject.getMass());
	otherObject.setVelocity(otherObject.getVelocity() - impulse / otherObject.getMass());

	// separate overlapping hitboxes
	float overlap = (mainObject.getRadius() + otherObject.getRadius()) - distance;
	if(overlap > 0.0f)
	{
		// mass based separation, lighter moves more
		float totalMass = mainObject.getMass() + otherObject.getMass();
		float separation1 = overlap * (otherObject.getMass() / totalMass);
		float separation2 = overlap * (mainObject.getMass() / totalMass);

		mainObject.setPosition(mainObject.getPosition() - normal * separation1);
		otherObject.setPosition(otherObject.getPosition() - normal * separation2);
	}
}

void detectAndHandleCollision(PhysicalObject& mainObject, std::vector<PhysicalObject>& physicalObjects, float deltaTime)
{
	// offset to measure from center of object
	sf::Vector2f mainPosition = mainObject.getPosition();
	float mainRadius = mainObject.getRadius();

	for(auto& obj : physicalObjects)
	{
		if(mainObject.getObjectID() == obj.getObjectID()) continue;

		sf::Vector2f objPosition = obj.getPosition();
		float objRadius = obj.getRadius();

		// shortest distance with wrapping
		sf::Vector2f delta = objPosition - mainPosition;
		if(std::abs(delta.x) > worldWidth / 2.0f)
		{
			delta.x = delta.x > 0 ? delta.x - worldWidth : delta.x + worldWidth;
		}
		if(std::abs(delta.y) > worldHeight / 2.0f)
		{
			delta.y = delta.y > 0 ? delta.y - worldHeight : delta.y + worldHeight;
		}

		// float distance = std::sqrt(std::pow((objPosition.y - mainPosition.y), 2) + std::pow((objPosition.x - mainPosition.x), 2));
		float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
		if(distance <= (mainRadius + objRadius))
		{
			handleCollision(mainObject, obj, deltaTime, restitution);
		}
	}
	// std::cout << std::endl;
}

void addDragForce(sf::Vector2f& currentVelocity, float dragCoef, float mass, float deltaTime)
{
	sf::Vector2f dragForce = {0, 0};
	dragForce.x = (1.0f - dragCoef * deltaTime / mass);
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
			fAccel.x = (acceleration / mass) * std::cos(direction) * deltaTime;
			fAccel.y = (-acceleration / mass) * std::sin(direction) * deltaTime;
		} else if(backward) // reverse direction
		{
			fAccel.x = (-0.5f * acceleration / mass) * std::cos(direction) * deltaTime;
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
}

void updateGame(sf::RenderWindow& window, Player& player, bool* (&buttons)[numButtons], std::vector<VisualObject>& visualObjects, std::vector<PhysicalObject>& physicalObjects)
{
	// update visual objects
	for(auto& obj : visualObjects)
	{
		obj.update(FixedDeltaTime, player.getVelocity());
	}
	
	// update physical objects
	for(auto& obj : physicalObjects)
	{
		obj.update(physicalObjects);
	}

	player.update(buttons, physicalObjects);
}

void drawGame(sf::RenderWindow& window, sf::View& view, Player& player, std::vector<VisualObject>& visualObjects, std::vector<PhysicalObject>& physicalObjects)
{
	// clear the window, then draw, then display
	window.clear();

	// Get view in correct spot
	view.setCenter(player.getPosition());
	window.setView(view);

	// draw visual objects
	for(auto& obj : visualObjects)
	{
		window.draw(obj.getSprite());
	}
	
	// draw physical objects
	for(auto& obj : physicalObjects)
	{
		obj.draw(window);
	}


	// draw the player
	player.draw(window);
	window.display();
}

void setupVisualObjects(std::vector<VisualObject>& objects)
{
	// far background
	sf::Vector2i bSize(1000 * windowWidth, 1000 * windowHeight);
	sf::Vector2f bPosition(-bSize.x/2.f, -bSize.y/2.f);
	float bRotation = 90;
	RenderLayer bRenderLayer = RenderLayer::FarBackground;
	RenderLayer bRenderLayer2 = RenderLayer::CloseBackground;
	std::string bgtFilename = "art/basicBackground.png";
	std::string bstFilename = "art/basicStars.png";
	VisualObject bg(bPosition, bSize, bRotation, bRenderLayer, bgtFilename);
	VisualObject fg(bPosition, bSize, bRotation, bRenderLayer2, bstFilename);

	objects.push_back(bg);
	objects.push_back(fg);
}

void setupPhysicalObjects(std::vector<PhysicalObject>& objects)
{
	// testing meteor
	sf::Vector2i objectSize(17, 17);
	float objectRotation = 90;
	RenderLayer objectRenderLayer = RenderLayer::Main;
	float objectMass = 10;
	float objectRadius = 8;
	sf::Vector2f objectVelocity = {0, 0};
	float objectAcceleration = 0;
	float objectDragCoef = 4;
	float objectRotationVelocity = 4;
	std::string objectFilename = "art/basicMeteor.png";
	float objectMaximumVelocity = 500; // equal to players for now

	// create 10 objects randomely on the first screen
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> distX(0.f, (float)windowWidth);
	std::uniform_real_distribution<float> distY(0.f, (float)windowHeight);

	// custom objects
	std::vector<sf::Vector2f*> mPositions[2];
		objects.push_back(PhysicalObject(sf::Vector2f(0.1, 0.1), objectSize, objectRotation, objectRenderLayer, objectFilename, objectMass, objectRadius, objectVelocity, objectAcceleration, objectDragCoef, objectRotationVelocity, objectMaximumVelocity));
		objects.push_back(PhysicalObject(sf::Vector2f(worldWidth - 2*objectRadius, 0.1), objectSize, objectRotation, objectRenderLayer, objectFilename, objectMass, objectRadius, objectVelocity, objectAcceleration, objectDragCoef, objectRotationVelocity, objectMaximumVelocity));

	for(int i = 0; i < 10; i++)
	{
		// random objects
		objects.push_back(PhysicalObject(sf::Vector2f(distX(rng), distY(rng)), objectSize, objectRotation, objectRenderLayer, objectFilename, objectMass, objectRadius, objectVelocity, objectAcceleration, objectDragCoef, objectRotationVelocity, objectMaximumVelocity));
	}
}

int main()
{
	// keys
	bool escPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;
	bool* buttons[numButtons];
	buttons[0] = &escPressed;
	buttons[1] = &upPressed;
	buttons[2] = &downPressed;
	buttons[3] = &leftPressed;
	buttons[4] = &rightPressed;

	// setup game objects
	
	// player
	sf::Vector2f pPosition(900, 500);
	sf::Vector2i pSize(29, 30);
	float pRotation = 90;
	RenderLayer pRenderLayer = RenderLayer::Main;
	std::string pFilename = "art/basicSpriteL.png";
	float pMass = 10;
	float pRadius = 14;
	sf::Vector2f pVelocity = {0, 0};
	float pAcceleration = 5000;
	float pRotationVelocity = 4;
	float pDrag = 4;
	float pMaxVelocity = 500;
	Player player = Player(pPosition, pSize, pRotation, pRenderLayer, pFilename, pMass, pRadius, pVelocity, pAcceleration, pDrag, pRotationVelocity, pMaxVelocity);

	// visual
	std::vector<VisualObject> visualObjects;
	setupVisualObjects(visualObjects);

	// physical
	std::vector<PhysicalObject> physicalObjects;
	setupPhysicalObjects(physicalObjects);

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
			updateGame(window, player, buttons, visualObjects, physicalObjects);
			timeAccumulator -= FixedDeltaTime;
		}
		drawGame(window, playerView, player, visualObjects, physicalObjects);
	}
	return 0;
}
