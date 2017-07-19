#include "terrainEffect.h"
#include <iostream>

/*****************************************************************************************************************/
//		Prosta efekt obiektówki dla broni typu explosive, 


void ProjectileTerrainEffect::update() 
{
	// powiêkszamy obszar 
	double newRadius = this->func(animCounter, this->durationTime) * maxRadiusSize;
	if (this->circle.getRadius() != newRadius)
	{
		diffBtwNewAndOldCircle = newRadius - circle.getRadius();
		this->circle.setRadius(newRadius);
	}

	if (diffBtwNewAndOldCircle)
		this->circle.move(-diffBtwNewAndOldCircle, -diffBtwNewAndOldCircle);
	
	this->diffBtwNewAndOldCircle = newRadius;

	animCounter += funcStep;
	


	// je¿eli przekroczono czas to dajemy znaæ silnikowi ¿e czas usun¹æ obszarkówkê
	if (this->animationClock.getElapsedTime().asSeconds() > this->durationTime || circle.getRadius() < 0)
	{
		this->needToDelete = true;
	}
}

void ProjectileTerrainEffect::create(terrainEffectParams & params, sf::Vector2f pos)
{
	this->type = params.type;
	this->animationClock.restart();
	this->animCounter = 0;
	this->funcStep = params.step;
	this->func = params.function;
	this->durationTime = params.durationOfAnimation;
	this->damage = params.damage;
	this->maxRadiusSize = params.maxRadiusSize;
	this->circle.setRadius(0);
	this->diffBtwNewAndOldCircle = 0;

	this->circle.setPosition(pos);
	this->needToDelete = false;

	switch (this->type)
	{
	case typeOfTerrainEffects::ELECTRIC:
		this->circle.setFillColor(sf::Color::Color(0x0, 0x40, 0xff));
		break;
	case typeOfTerrainEffects::EXPLOSIVE:
		this->circle.setFillColor(sf::Color::Color(0xff,0xb3, 0x66));
		break;
	case typeOfTerrainEffects::FIRE:
		this->circle.setFillColor(sf::Color::Color(0xff,0x30,0x30));
		break;
	case typeOfTerrainEffects::TOXIC:
		this->circle.setFillColor(sf::Color::Color(0x0,0xff,0x80));
		break;
	}

}

void ProjectileTerrainEffect::create(terrainEffectParams & params, std::vector<projectile>& projectileArray)
{
	
}

/*****************************************************************************************************************/