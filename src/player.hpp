#pragma once
#include <SFML\Graphics.hpp>

class player : public sf::Sprite
{
public:
	player(float radius, sf::Color color, sf::Vector2f position) :
		SightLine(sf::Lines, 2)
	{

		movementSpeed = 7.f;

		circle.setRadius(radius / 2);
		circle.setFillColor(color);
		circle.setPosition(position);
		this->setPosition(position);

		//texture.loadFromFile("bohater.png");
		//this->setTexture(texture);

		this->circle.setOrigin(circle.getRadius(), circle.getRadius());
		this->setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);

		// ustawiamy parametry pocz¹tkowe 
		this->parameters.agility = 1;
		this->parameters.perception = 1;
		this->parameters.strength = 1;
		this->parameters.rethoric = 1;

		// ustawienie pocz¹tkowych perków na false 
		this->parameters.fastHands = false;
		this->parameters.sniperPerk = false;

	}
	void update(sf::Vector2i mousePosition)
	{

		manageKeyboardInput();
		circle.setPosition(this->getPosition());

		// wyliczamy sobie z arc tangesa k¹t  
		angle = atan2(mousePosition.y - this->circle.getPosition().y - this->circle.getRadius(),
			mousePosition.x - this->circle.getPosition().x - this->circle.getRadius());

		// jako ¿e arc tan zwraca wartoœæ w radianach to zmieniamy k¹t na stopnie
		angle = angle * 180 / M_PI + 90;

		circle.setRotation(angle);
		this->setRotation(angle);

		SightLine[0].position = sf::Vector2f(circle.getPosition().x, circle.getPosition().y);
		SightLine[1].position = sf::Vector2f(mousePosition);

	}
	sf::CircleShape getCircle()
	{
		return circle;
	}
	sf::VertexArray getLine() {
		return SightLine;
	}
	double getVx() {
		return movementSpeed;
	}
	double getVy()
	{
		return movementSpeed;
	}
	double getAngle()
	{
		return angle;
	}
	Verticle getCenter()
	{
		// jako ¿e ustawiliœmy wczeœniej origin center na œrodek
		// to nie musimy dodawaæ do pozycji promienia 
		return Verticle{ circle.getPosition().x, circle.getPosition().y };
	}
	playerParameters getParameters() const
	{
		return this->parameters;
	}

private:
	void manageKeyboardInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			this->move(-movementSpeed, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			this->move(movementSpeed, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			this->move(0, -movementSpeed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			this->move(0, movementSpeed);
		}
	}

	double movementSpeed, angle;
	sf::CircleShape  circle;
	sf::Texture      texture;
	sf::VertexArray  SightLine;


	playerParameters parameters;
};
