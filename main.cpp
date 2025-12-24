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

class Player;

float degreesToRadians(float degrees)
{
	return (degrees * (M_PI / 180));
}

float radiansToDegrees(float radians)
{
	return (radians * (180 / M_PI));
}

void addDragForce(sf::Vector2f& currentVelocity, float dragCoef)
{
	currentVelocity += {-dragCoef * currentVelocity.x, -dragCoef * currentVelocity.y};
	return;
}

void addAccelerationForce(sf::Vector2f& currentVelocity, float accelerationMagnitude, float direction, float maximumVelocity, int accelerate)
{
		// force of acceleration
		sf::Vector2f fAccel = {0, 0};
		if(accelerate == 1)
		{
			fAccel = {accelerationMagnitude * std::cos(direction), -accelerationMagnitude * std::sin(direction)};
		}
		else if(accelerate == -1)
		{
			// reverse should be slower by factor 0.75
			fAccel = {0.75f * -accelerationMagnitude * std::cos(direction), 0.75f * accelerationMagnitude * std::sin(direction)};
		}

		// range of possible velocitys to stay inbetween
		float maxXVelocity = maximumVelocity * std::cos(direction);
		float maxYVelocity = maximumVelocity * std::sin(direction);
		float minXVelocity = -maximumVelocity * std::cos(direction);
		float minYVelocity = -maximumVelocity * std::sin(direction);

		// make sure we're not going too fast
		// max/mins are based on quadrant
		float currentDirection = std::atan(currentVelocity.y / currentVelocity.x);
		if(direction <= M_PI/2) // Q1
		{
			if(currentVelocity.x < maxXVelocity && currentVelocity.x > minXVelocity)
			{
				currentVelocity.x += fAccel.x;
			}
			if(currentVelocity.y < maxYVelocity && currentVelocity.y > minYVelocity)
			{
				currentVelocity.y += fAccel.y;
			}
		}
		else if(direction <= M_PI) // Q2
		{
			if(currentVelocity.x > maxXVelocity && currentVelocity.x < minXVelocity)
			{
				currentVelocity.x += fAccel.x;
			}
			if(currentVelocity.y < maxYVelocity && currentVelocity.y > minYVelocity)
			{
				currentVelocity.y += fAccel.y;
			}
		}
		else if(direction <= 3 * M_PI / 2) // Q3
		{
			if(currentVelocity.x > maxXVelocity && currentVelocity.x < minXVelocity)
			{
				currentVelocity.x += fAccel.x;
			}
			if(currentVelocity.y > maxYVelocity && currentVelocity.y < minYVelocity)
			{
				currentVelocity.y += fAccel.y;
			}
		}
		else // Q4
		{
			if(currentVelocity.x < maxXVelocity && currentVelocity.x > minXVelocity)
			{
				currentVelocity.x += fAccel.x;
			}
			if(currentVelocity.y > maxYVelocity && currentVelocity.y < minYVelocity)
			{
				currentVelocity.y += fAccel.y;
			}
		}
}

class Arena
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

	Arena(sf::Vector2i size, sf::Vector2f position, const sf::Texture& bgTexture, const sf::Texture& stTexture)
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

class Player
{
	sf::Vector2i m_size;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_rotation;

	float m_mVelocity;

	float m_acceleration;
	float m_drag;
	float m_rotVelocity;

	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;

public:

	Player(sf::Vector2i size, sf::Vector2f position, sf::Vector2f velocity, float rotation, float maxVelocity, float acceleration, float drag, float rotationVelocity, const sf::Texture& texture)
			: m_size(size)
			  , m_position(position)
			  , m_velocity(velocity)
			  , m_rotation(rotation)
			  , m_mVelocity(maxVelocity)
			  , m_acceleration(acceleration)
			  , m_drag(drag)
			  , m_rotVelocity(rotationVelocity)
			  , m_texture(texture)
			  , m_spriteRect(sf::Vector2i{0, 0},size)
			  , m_sprite(m_texture, m_spriteRect)
			  {
				  m_sprite.setOrigin({21, 25});
				  std::cout << "Player Constructed" << std::endl;
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

	float getAcceleration() const
	{
		return m_acceleration;
	}
	float getdrag() const
	{
		return m_drag;
	}
	float getRotVelocity() const
	{
		return m_rotVelocity;
	}

	sf::Sprite getSprite()
	{
		return m_sprite;
	}

	void updateVelocity(int accelerate)
	{
		// zero out the velocity if its small enough
		if(m_velocity.x < 0.1 && m_velocity.x > -0.1) m_velocity.x = 0;
		if(m_velocity.y < 0.1 && m_velocity.y > -0.1) m_velocity.y = 0;

		// get the angle the player is facing
		float forward = m_rotation;
		forward = degreesToRadians(forward);

		// add drag and acceleration forces
		addDragForce(m_velocity, m_drag);
		addAccelerationForce(m_velocity, m_acceleration, forward, getMaxVelocity(), accelerate);

		std::cout << std::endl;
		std::cout << "Velocity: " << m_velocity.x << ", " << m_velocity.y << std::endl;
		std::cout << "Position: " << m_position.x << ", " << m_position.y << std::endl;
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

	void update(float deltaTime, sf::RenderWindow& window, bool* (&buttons)[numButtons], Player& p, Arena& a)
	{
		// handle rotation
		float potentialRotation = 0;
		if(*buttons[3]) 
		{
			potentialRotation -= p.getRotVelocity();
		}
		if(*buttons[4]) 
		{
			potentialRotation += p.getRotVelocity();
		}
		if(potentialRotation != 0) 
			p.rotate(potentialRotation);

		// handle acceleration
		sf::Vector2f curVelocity = p.getVelocity();
		int accel = 0;
		if(*buttons[1] && !*buttons[2])
		{
			// std::cout << "Accelerating" << std::endl;
			accel = 1;
		}
		else if(*buttons[2] && !*buttons[1])
		{
			// std::cout << "decelerating" << std::endl;
			accel = -1;
		}
		p.updateVelocity(accel);
		p.updatePosition(deltaTime);

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

void updateGame(float deltaTime, sf::RenderWindow& window, bool* (&buttons)[numButtons], Player& p, Arena& a)
{
	p.update(deltaTime, window, buttons, p, a);
	a.update(deltaTime, p.getVelocity());
}

void drawGame(sf::RenderWindow& window, Player& p, Arena& a)
{
		window.clear();
		window.draw(a.getbgSprite());
		window.draw(a.getstSprite());
		window.draw(p.getSprite());
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
	sf::RenderWindow window(sf::VideoMode(wSize), "Very cool and also fun game");
	window.setFramerateLimit(60);
	window.setPosition({1930, 10}); // pretty arbitrary, top right of my second monitor

	

	// arena
	sf::Vector2i aSize(100 * windowWidth, 100 * windowHeight);
	sf::Vector2f aPosition(-aSize.x/2.f, -aSize.y/2.f);
	sf::Texture arenaBGTexture;
	if(!arenaBGTexture.loadFromFile("art/basicBackground.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	arenaBGTexture.setRepeated(true);
	sf::Texture arenaSTTexture;
	if(!arenaSTTexture.loadFromFile("art/basicStars.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	arenaSTTexture.setRepeated(true);
	Arena a(aSize, aPosition, arenaBGTexture, arenaSTTexture);

	// player
	sf::Vector2i pSize(41, 41);
	sf::Vector2f pPosition(900, 500);
	sf::Vector2f pVelocity = {0, 0};
	float pRotation = 90;
	float pMaxVelocity = 500;
	float pAcceleration = 6;
	float pdrag = 0.005;
	float pRotationVelocity = 4;
	sf::Texture playerTexture;
	if(!playerTexture.loadFromFile("art/basicSpriteL.png"))
		std::cout << "Sprite not loaded :(" << std::endl;
	Player p(pSize, pPosition, pVelocity, pRotation, pMaxVelocity, pAcceleration, pdrag, pRotationVelocity, playerTexture);

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
			updateGame(fixedDt, window, buttons, p, a);
			timeAccumulator -= fixedDt;
		}
		playerView.setCenter(p.getPosition());
		window.setView(playerView);
		drawGame(window, p, a);
	}
	window.close();
	return 0;
}
