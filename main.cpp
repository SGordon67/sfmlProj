#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cmath>

const int numButtons = 5;
const int windowWidth = 1500;
const int windowHeight = 750;
int numObjects = 0;

float degreesToRadians(float degrees)
{
	return (degrees * (M_PI / 180));
}

float radiansToDegrees(float radians)
{
	return (radians * (180 / M_PI));
}

void enforceMaximumVelocity(sf::Vector2f& currentVelocity, sf::Vector2f fAccel, float direction, float maximumVelocity)
{
	// range of possible velocitys to stay inbetween
	float maxXVelocity = maximumVelocity * std::cos(direction);
	float maxYVelocity = maximumVelocity * std::sin(direction);
	float minXVelocity = -maximumVelocity * std::cos(direction);
	float minYVelocity = -maximumVelocity * std::sin(direction);

	std::cout << "Maximum velocity allowed: " << maximumVelocity << ", (" << maxXVelocity << ", " << maxYVelocity << ") and (" << minXVelocity << ", " << minYVelocity << ")" << std::endl;
	std::cout << "Current velocity " << currentVelocity.x << ", " << currentVelocity.y << std::endl;

	// make sure we're not going too fast
	// max/mins are based on quadrant
	// quadrant facing
	int quad = 0;
	if(direction <= M_PI/2)
		quad = 1;
	else if(direction <= M_PI)
		quad = 2;
	else if(direction <= 3 * M_PI / 2)
		quad = 3;
	else
		quad = 4;
	switch(quad)
	{
		case 1:
			if(currentVelocity.x > maxXVelocity)
			{
				// std::cout << "Maximum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			else if(currentVelocity.x < minXVelocity)
			{
				// std::cout << "Minimum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			if(currentVelocity.y > maxYVelocity)
			{
				// std::cout << "Maximum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			else if(currentVelocity.y < minYVelocity)
			{
				// std::cout << "Minimum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			break;
		case 2:
			if(currentVelocity.x < maxXVelocity)
			{
				// std::cout << "Maximum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			else if(currentVelocity.x > minXVelocity)
			{
				// std::cout << "Minimum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			if(currentVelocity.y > maxYVelocity)
			{
				// std::cout << "Maximum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			else if(currentVelocity.y < minYVelocity)
			{
				// std::cout << "Minimum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			break;
		case 3:
			if(currentVelocity.x < maxXVelocity)
			{
				// std::cout << "Maximum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			else if(currentVelocity.x > minXVelocity)
			{
				// std::cout << "Minimum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			if(currentVelocity.y < maxYVelocity)
			{
				// std::cout << "Maximum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			else if(currentVelocity.y > minYVelocity)
			{
				// std::cout << "Minimum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			break;
		case 4:
			if(currentVelocity.x > maxXVelocity)
			{
				// std::cout << "Maximum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			else if(currentVelocity.x < minXVelocity)
			{
				// std::cout << "Minimum X Speed Reached" << std::endl;
				currentVelocity.x -= fAccel.x;
			}
			if(currentVelocity.y < maxYVelocity)
			{
				// std::cout << "Maximum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			else if(currentVelocity.y > minYVelocity)
			{
				// std::cout << "Minimum Y Speed Reached" << std::endl;
				currentVelocity.y -= fAccel.y;
			}
			break;
	}
}

void addDragForce(sf::Vector2f& currentVelocity, float dragCoef)
{
	std::cout << "Drag Force: " << -dragCoef * currentVelocity.x << ", " << -dragCoef * currentVelocity.y << std::endl;
	currentVelocity += {-dragCoef * currentVelocity.x, -dragCoef * currentVelocity.y};
	return;
}

void addAccelerationForce(sf::Vector2f& currentVelocity, float accelerationMagnitude, float direction, float maximumVelocity)
{
		// force of acceleration
		sf::Vector2f fAccel = {0, 0};
		if(accelerationMagnitude != 0)
		{
			if(accelerationMagnitude > 0){
				fAccel = {accelerationMagnitude * std::cos(direction), -accelerationMagnitude * std::sin(direction)};
			}
			if(accelerationMagnitude < 0){
				// reverse is slower to reach max velocity
				fAccel = {0.5f*accelerationMagnitude * std::cos(direction), -0.5f*accelerationMagnitude * std::sin(direction)};
			}
		}

		currentVelocity.x += fAccel.x;
		currentVelocity.y += fAccel.y;

		std::cout << "Acceleration Force: " << fAccel.x << ", " << fAccel.y << std::endl;
	
		enforceMaximumVelocity(currentVelocity, fAccel, direction, maximumVelocity);
}

class Background
{
	sf::Vector2i m_size;
	sf::Vector2f m_position;

	sf::Texture m_bgTexture;
	sf::IntRect m_bgSpriteRect;
	sf::Sprite m_bgSprite;

	sf::Texture m_stTexture;
	sf::IntRect m_stSpriteRect;
	sf::Sprite m_stSprite;

public:

	Background(sf::Vector2i size, sf::Vector2f position, const sf::Texture& bgTexture, const sf::Texture& stTexture)
			: m_size(size)
			  , m_position(position)
			  , m_bgTexture(bgTexture)
			  , m_bgSpriteRect(sf::Vector2i{-size.x/2, size.y/2}, size)
			  , m_bgSprite(m_bgTexture, m_bgSpriteRect)
			  , m_stTexture(stTexture)
			  , m_stSpriteRect(sf::Vector2i{-size.x/2, size.y/2}, size)
			  , m_stSprite(m_stTexture, m_stSpriteRect)
	{
		m_bgSprite.setPosition({m_position.x, m_position.y});
		m_stSprite.setPosition({m_position.x, m_position.y});
	}

	sf::Sprite getbgSprite()
	{
		return m_bgSprite;
	}

	sf::Sprite getstSprite()
	{
		return m_stSprite;
	}

	void update(float deltaTime, sf::Vector2f playerVelocity)
	{
		// move the background at different rates in relation to the player for a feeling of depth
		m_bgSprite.move(0.5f * playerVelocity * deltaTime);

		// starts keeping static for movement reference (feels good though)
		// m_stSprite.move(0.8f * playerVelocity * deltaTime);
		return;
	}
};

class PhysicsObject
{
protected:
	int m_objectID;

	sf::Vector2i m_size;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_rotation;

	float m_mVelocity;

	float m_drag;
	float m_rotVelocity;

	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;

public:

	PhysicsObject(sf::Vector2i size, sf::Vector2f position, sf::Vector2f velocity, float rotation, float maxVelocity, float drag, float rotationVelocity, const sf::Texture& texture)
			: m_objectID(++numObjects)
			  , m_size(size)
			  , m_position(position)
			  , m_velocity(velocity)
			  , m_rotation(rotation)
			  , m_mVelocity(maxVelocity)
			  , m_drag(drag)
			  , m_rotVelocity(rotationVelocity)
			  , m_texture(texture)
			  , m_spriteRect(sf::Vector2i{0, 0},size)
			  , m_sprite(m_texture, m_spriteRect)
			  {
				  m_sprite.setOrigin({size.x / 2.f, size.y / 2.f});
				  std::cout << "PhysicsObject Constructed" << std::endl;
			  }

	void updateVelocity()
	{
		// zero out the velocity if its small enough
		if(m_velocity.x < 0.5 && m_velocity.x > -0.5) m_velocity.x = 0;
		if(m_velocity.y < 0.5 && m_velocity.y > -0.5) m_velocity.y = 0;

		// get the angle the player is facing
		float forward = m_rotation;
		forward = degreesToRadians(forward);

		// add drag and acceleration forces
		addDragForce(m_velocity, m_drag);

		std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
		std::cout << std::endl;
		return;
	}

	void updatePosition(const float deltaTime)
	{
		// update the position
		m_position += m_velocity * deltaTime;
		m_sprite.setPosition({m_position.x, m_position.y});
		return;
	}

	void rotate(const float rotVal)
	{
		// std::cout << "Rotating player by rotVal: " << rotVal << std::endl;
		m_sprite.rotate(sf::degrees(rotVal));
		m_rotation -= rotVal;
		if(m_rotation >= 360) m_rotation -= 360;
		if(m_rotation < 0) m_rotation += 360;
		return;
	}

	void update(float deltaTime, bool* (&buttons)[numButtons])
	{
		// handle rotation
		float potentialRotation = 0;
		if(*buttons[3]) 
		{
			potentialRotation -= m_rotVelocity;
		}
		if(*buttons[4]) 
		{
			potentialRotation += m_rotVelocity;
		}
		if(potentialRotation != 0) 
			this->rotate(potentialRotation);

		// handle acceleration
		sf::Vector2f curVelocity = m_velocity;
		this->updateVelocity();
		this->updatePosition(deltaTime);

	}

	sf::Vector2f getPosition() const
	{
		return m_position;
	}
	sf::Vector2f getVelocity() const
	{
		return m_velocity;
	}
	float getRotation() const
	{
		return m_rotation;
	}

	float getMaxVelocity() const
	{
		return m_mVelocity;
	}

	float getdrag() const
	{
		return m_drag;
	}

	float getRotVelocity() const
	{
		return m_rotVelocity;
	}

	sf::Sprite getSprite() const
	{
		return m_sprite;
	}
};

class Player : public PhysicsObject
{
	float m_acceleration;
public:

	Player(sf::Vector2i size, sf::Vector2f position, sf::Vector2f velocity, float rotation, float maxVelocity, float acceleration, float drag, float rotationVelocity, const sf::Texture& texture)
			: PhysicsObject(size, position, velocity, rotation, maxVelocity, drag, rotationVelocity, texture)
			  , m_acceleration(acceleration)
			  {
				  m_sprite.setOrigin({21, 27});
				  std::cout << "Player Constructed" << std::endl;
			  }


	void updateVelocity(float accelerate)
	{
		// zero out the velocity if its small enough
		if(m_velocity.x < 0.1 && m_velocity.x > -0.1) m_velocity.x = 0;
		if(m_velocity.y < 0.1 && m_velocity.y > -0.1) m_velocity.y = 0;

		// get the angle the player is facing
		float forward = m_rotation;
		forward = degreesToRadians(forward);

		// add drag and acceleration forces
		addDragForce(m_velocity, m_drag);
		addAccelerationForce(m_velocity, accelerate, forward, getMaxVelocity());

		std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
		std::cout << std::endl;
		return;
	}

	void update(float deltaTime, bool* (&buttons)[numButtons])
	{
		// handle rotation
		float potentialRotation = 0;
		if(*buttons[3]) 
		{
			potentialRotation -= m_rotVelocity;
		}
		if(*buttons[4]) 
		{
			potentialRotation += m_rotVelocity;
		}
		if(potentialRotation != 0) 
			this->rotate(potentialRotation);

		// handle acceleration
		sf::Vector2f curVelocity = m_velocity;
		int accel = 0;
		if(*buttons[1] && !*buttons[2])
		{
			accel = m_acceleration;
		}
		if(*buttons[2] && !*buttons[1])
		{
			// std::cout << "decelerating" << std::endl;
			accel = -m_acceleration;
		}
		this->updateVelocity(accel);
		this->updatePosition(deltaTime);

	}
};

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

void updateGame(float deltaTime, sf::RenderWindow& window, bool* (&buttons)[numButtons], Player& player, Background& background)
{
	player.update(deltaTime, buttons);
	background.update(deltaTime, player.getVelocity());
}

void drawGame(sf::RenderWindow& window, Player& player, Background& background)
{
		window.clear();
		window.draw(background.getbgSprite());
		window.draw(background.getstSprite());
		window.draw(player.getSprite());
		window.display();
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

	// window
	sf::Vector2<uint> wSize(windowWidth, windowHeight);
	sf::RenderWindow window(sf::VideoMode(wSize), "game?");
	window.setFramerateLimit(60);
	window.setPosition({1930, 10}); // pretty arbitrary for now, top right of my second monitor

	// background
	sf::Vector2i bSize(1000 * windowWidth, 1000 * windowHeight);
	sf::Vector2f bPosition(-bSize.x/2.f, -bSize.y/2.f);
	sf::Texture backgroundBGTexture;
	if(!backgroundBGTexture.loadFromFile("art/basicBackground.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	backgroundBGTexture.setRepeated(true);
	sf::Texture backgroundSTTexture;
	if(!backgroundSTTexture.loadFromFile("art/basicStars.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	backgroundSTTexture.setRepeated(true);
	Background bg(bSize, bPosition, backgroundBGTexture, backgroundSTTexture);

	// arena
	

	// player
	sf::Vector2i pSize(41, 41);
	sf::Vector2f pPosition(900, 500);
	sf::Vector2f pVelocity = {0, 0};
	float pRotation = 90;
	// float pMaxVelocity = 1000;
	float pMaxVelocity = 1000; // for testing limits
	float pAcceleration = 6;
	float pdrag = 0.005;
	float pRotationVelocity = 4;
	sf::Texture playerTexture;
	if(!playerTexture.loadFromFile("art/basicSpriteL.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	Player p(pSize, pPosition, pVelocity, pRotation, pMaxVelocity, pAcceleration, pdrag, pRotationVelocity, playerTexture);


	// testing object
	sf::Vector2i o1Size(40, 40);
	sf::Vector2f o1Position(900, 400);
	sf::Vector2f o1Velocity = {0, 0};
	float o1Rotation = 90;
	float o1MaxVelocity = 1000;
	float o1Drag = 0.01;
	float o1RotationVelocity = 4;
	sf::Texture o1Texture;
	if(!o1Texture.loadFromFile("art/basicMeteor.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	PhysicsObject o1(o1Size, o1Position, o1Velocity, o1Rotation, o1MaxVelocity, o1Drag, o1RotationVelocity, o1Texture);

	// list of all objects to be drawn
	std::vector<PhysicsObject> gameObjects;
	gameObjects.push_back(p);
	gameObjects.push_back(o1);

	// time
	sf::Clock clock;
	const float fixedDt = 1.0f / 60.0f; // fixed delta time
	float timeAccumulator = 0.0f;

	// view
	sf::View playerView(p.getPosition()+p.getSprite().getOrigin(), {1500, 750});
	window.setView(playerView);

	while(window.isOpen())
	{
		// time since last frame, maximum of 0.25 enforced to prevent edge cases
		float frameTime = clock.restart().asSeconds();
		if(frameTime > 0.25f)
			frameTime = 0.25f;
		timeAccumulator += frameTime;

		updateButtonPresses(buttons);
		while(const std::optional event = window.pollEvent())
		{
			if(event->is<sf::Event::Closed>() || escPressed)
			{
				window.close();
			}
		}

		// if more time has passed than Dt, we can progress the game
		while(timeAccumulator >= fixedDt)
		{
			updateGame(fixedDt, window, buttons, p, bg);
			timeAccumulator -= fixedDt;
		}
		playerView.setCenter(p.getPosition());
		window.setView(playerView);
		drawGame(window, p, bg);
	}
	window.close();
	return 0;
}
