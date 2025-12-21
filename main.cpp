#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

class Player
{
	sf::Vector2i m_size;

	sf::Vector2f m_position;
	sf::Vector2f  m_speed;
	sf::Vector2f m_acceleration;

	float m_maxSpeed;
	float m_maxAcceleration;

	float m_rotation;
	float m_rotationSpeed;
	
	sf::Texture m_texture;
	sf::IntRect m_spriteRect;
	sf::Sprite m_sprite;

public:

	Player(sf::Vector2i size, sf::Vector2f position, float maxSpeed, float maxAcceleration, float rotation, float rotationSpeed, const sf::Texture& texture)
			: m_size(size)
			  , m_position(position)
			  , m_speed(sf::Vector2f(0, 0))
			  , m_acceleration(sf::Vector2f(0, 0))
			  , m_maxSpeed(maxSpeed)
			  , m_maxAcceleration(maxAcceleration)
			  , m_rotation(rotation)
			  , m_rotationSpeed(rotationSpeed)
			  , m_texture(texture)
			  , m_spriteRect(sf::Vector2i{31, 31},size)
			  , m_sprite(m_texture, m_spriteRect)

	{
		//pSprite.setTextureRect(pSpriteRect);
		m_sprite.setOrigin({15.f, 15.f});
		std::cout << "Player Constructed" << std::endl;
	}

	sf::Sprite getSprite()
	{
		return m_sprite;
	}
	void rotate(bool right)
	{
		if(right)
		{
			m_sprite.rotate(sf::degrees(m_rotationSpeed));
		} else
		{
			m_sprite.rotate(sf::degrees(-1*m_rotationSpeed));
		}
	}
};

class Arena
{
	sf::Vector2i m_size;

public:

	Arena(sf::Vector2i size)
			: m_size(size)
	{
	}
};


void updateGame(sf::Time elapsed, Player& p, Arena& a, sf::RenderWindow& window)
{
	std::cout << "In the new update function" << std::endl;

}

int main()
{
	// keys
	bool escPressed = false;
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;

	sf::Vector2i aSize(15000, 7500);
	sf::Vector2<uint> wSize(1500, 750);

	sf::Vector2i pSize(30, 30);
	sf::Vector2f pPosition(50, 50);
	float pMaxSpeed = 10;
	float pMaxAcceleration = 5;
	float pRotation = 0;
	float pRotationSpeed = 5;
	sf::Texture playerTexture;
	if(!playerTexture.loadFromFile("art/basicSprite.png"))
		std::cout << "Sprite not loaded :(" << std::endl;

	// create arena
	Arena a(aSize);

	// create player
	Player p(pSize, pPosition, pMaxSpeed, pMaxAcceleration, pRotation, pRotationSpeed, playerTexture);

	// create window
	sf::RenderWindow window(sf::VideoMode({wSize.x, wSize.y}), "Very cool and also fun game");
	window.setPosition({1930, 10});

	sf::Clock clock;

	window.setFramerateLimit(60);
	while(window.isOpen())
	{
		while(const std::optional event = window.pollEvent())
		{
			if(event->is<sf::Event::Closed>() || escPressed)
			{
				window.close();
			}
			else if(const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if(keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
				switch(keyPressed->scancode)
				{
					case sf::Keyboard::Scancode::Escape:
						escPressed = true;
						break;
					case sf::Keyboard::Scancode::Left:
					case sf::Keyboard::Scancode::A:
						std::cout<< "Action rotate left, kepressed: " << static_cast<int>(keyPressed->scancode) << std::endl;
						leftPressed = true;
						p.rotate(false);
						break;
					case sf::Keyboard::Scancode::Right:
					case sf::Keyboard::Scancode::D:
						std::cout<< "Action rotate right, kepressed: " << static_cast<int>(keyPressed->scancode) << std::endl;
						rightPressed = true;
						p.rotate(true);
						break;
					case sf::Keyboard::Scancode::Up:
					case sf::Keyboard::Scancode::W:
						std::cout<< "Action Accelerate, kepressed: " << static_cast<int>(keyPressed->scancode) << std::endl;
						upPressed = true;
						break;
					case sf::Keyboard::Scancode::Down:
					case sf::Keyboard::Scancode::S:
						std::cout<< "Action Decelerate, kepressed: " << static_cast<int>(keyPressed->scancode) << std::endl;
						downPressed = true;
						break;
				}
			}
		}
		sf::Time elapsed = clock.restart();
		updateGame(elapsed, p, a, window);
		window.clear();
		window.draw(p.getSprite());
		window.display();
	}
	window.close();
	return 0;
}
