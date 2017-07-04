#include "player.hpp"
#include "specialAbility.h"
#include <iostream>

/************************************************************************************/
player::player(float radius, sf::Color color, sf::Vector2f position, sf::RenderWindow &Window, std::vector<std::vector<enemy*>> &refToEnemyArray) :
	SightLine(sf::Lines, 2), refToMainWindow(Window), enemyArrayRef(refToEnemyArray)
{

	movementSpeed = 7.f;

	circle.setRadius(radius / 2);
	circle.setFillColor(color);
	circle.setPosition(position);
	this->setPosition(position);

	//texture.loadFromFile("bohater.png");
	//this->setTexture(texture);

	sf::Color HpBarColor = sf::Color::Color(0x0, 0xff, 0);

	this->HP = parameters.maxHP;
	prevHP = HP;


	this->circle.setOrigin(circle.getRadius(), circle.getRadius());
	this->setOrigin(this->getGlobalBounds().width / 2, this->getGlobalBounds().height / 2);

	this->initializeStartupParameters();
	this->RectHPOfPlayer.init(this->parameters.maxHP, this->refToMainWindow.getSize().x / 2, this->refToMainWindow.getSize().y / 16., HpBarColor);


	this->HPText.setFillColor(sf::Color(0xff, 0, 0));
	this->HPText.setString(std::to_string(cast(int, this->HP)) + "/" + std::to_string(cast(int, this->parameters.maxHP)));
	this->HPText.setCharacterSize(getHPbar().getGlobalBounds().height);

	this->HPText.setPosition(sf::Vector2f(WINDOWWIDTH / 2. - HPText.getGlobalBounds().width / 2, this->refToMainWindow.getSize().y / 32 - HPText.getGlobalBounds().height / 2));
}

/************************************************************************************/
void player::update(sf::Vector2i mousePosition, sf::Vector2f windowOffset)
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

	if (this->HP != prevHP)
	{
		this->HPText.setString(std::to_string(cast(int,HP)) + "/" + std::to_string(cast(int,this->parameters.maxHP)));
		prevHP = HP;
	}

	sf::Vector2f newPos = sf::Vector2f(this->refToMainWindow.getSize().x / 2 + windowOffset.x - HPText.getGlobalBounds().width/2,
		windowOffset.y + this->refToMainWindow.getSize().y/16. - HPText.getGlobalBounds().height/2);

	if (newPos != this->HPText.getPosition())
	{
		HPText.setPosition(newPos);
	}
	this->RectHPOfPlayer.update(sf::Vector2f(this->refToMainWindow.getSize().x/4 + windowOffset.x, windowOffset.y + this->refToMainWindow.getSize().y/16), this->HP, false);

	
}

/************************************************************************************/
void player::dealDamage(damageParameters DmgParams)
{
	this->HP -= DmgParams.value;

	if (this->HP <= 0)
	{
		this->isThisAlive = false;
	}
}

/************************************************************************************/
void player::draw()
{
	refToMainWindow.draw(*this);
	refToMainWindow.draw(circle);
	refToMainWindow.draw(RectHPOfPlayer);
	refToMainWindow.draw(HPText);
	refToMainWindow.draw(SightLine);
}


/************************************************************************************/
void player::initializeStartupParameters()
{
	// ustawiamy parametry pocz¹tkowe 
	this->parameters.agility = 1;
	this->parameters.perception = 1;
	this->parameters.strength = 1;
	this->parameters.rethoric = 1;

	// ustawienie pocz¹tkowych perków na false 
	this->parameters.fastHands = false;
	this->parameters.sniperPerk = false;
	this->parameters.maxHP = 100;
	this->HP = 100;
	this->parameters.criticalShotChance = 5;
	this->parameters.criticalShotMultipler = 2.;
}

/************************************************************************************/
void player::manageSpecialAbilities()
{
}
 
/************************************************************************************/
void player::manageKeyboardInput()

{
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		&& this->getPosition().x - this->circle.getRadius()  > LEFTWALL)
	{
		this->move(-movementSpeed, 0);
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		&& this->getPosition().x + this->circle.getRadius()< RIGHTWALL)
	{
		this->move(movementSpeed, 0);
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
		&& this->getPosition().y - this->circle.getRadius() > UPWALL)
	{
		this->move(0, -movementSpeed);
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		&& this->getPosition().y + this->circle.getRadius() < DOWNWALL)
	{
		this->move(0, movementSpeed);
	}
}

/************************************************************************************/
