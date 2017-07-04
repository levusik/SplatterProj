#pragma once

#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include "enemy.hpp"

class specialAbility : sf::Sprite
{
public:
	
	specialAbility();


	// settery 
	void setRectAndSpriteParams(sf::Vector2f size, sf::Vector2f position, sf::Color col = sf::Color::Color(0xff, 0xff, 0xff));
	void setTexture(sf::Texture &texture);
	void setPointerToEnemyArray(std::vector<std::vector<enemy*>> *enemyArrPtr);
	void setPointerToWindow(sf::RenderWindow *windowPtr);
	bool isStillActive();


	// gettery 
	sf::RectangleShape getRect() const;

	// metody wirtualne 
	virtual void activate()
	{}
	virtual void continueActingIfNecessary()
	{}
	virtual void drawAnimations()
	{}



protected:
	bool	isActive;
	typeOfSpecialAbility	type;
	sf::RectangleShape		rect;
	sf::RenderWindow		*windowPtr;
	damageParameters		DamageParameters;
	std::vector<std::vector<enemy *>> *enemyArrayPtr;

};

