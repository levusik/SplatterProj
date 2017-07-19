#pragma once

#include <SFML\Graphics.hpp>
#include <vector>


#include "entity.hpp"
#include "enemy.hpp"

// klasa abstrakcyjna obszarówki ( umiejêtoœci oraz specjalne pociski )


class terrainEffect
{
public: 
	terrainEffect()
	{
		this->animationClock.restart();
	}


	virtual void update()
	{
	}
	virtual void create(terrainEffectParams &params, sf::Vector2f pos)
	{}

	// kastety 
	virtual void create(terrainEffectParams &params, std::vector<projectile> &projectileArray)
	{}

	bool needToDeleteEffect()
	{
		return this->needToDelete;
	}

	damageParameters getDamage()
	{
		damageParameters params;
		params.value = this->damage;

		return params;
	}

	sf::Vector2f getCenter()
	{
		return sf::Vector2f(this->circle.getPosition().x + this->circle.getRadius(),
							this->circle.getPosition().y + this->circle.getRadius());
	}


	// gettery
	sf::CircleShape getCircle()
	{
		return this->circle;
	}
	sf::Sprite		getSprite()
	{
		return this->sprite;
	}


	// settery
	void setTexture(sf::Texture &texture)
	{
		this->sprite.setTexture(texture);
	}

	void setFunction( double(*Func)(double,int), int timeOfFunc, double step) 
	{
		this->func			= Func;
		this->timeOfFunc	= timeOfFunc;
		this->funcStep		= step;
	}
	void setParamsOfCircle(int radius, sf::Color fillColor)
	{
		this->circle.setRadius(radius);
		this->circle.setFillColor(fillColor);
		this->circle.setOrigin(radius, radius);
	}

protected:
	typeOfTerrainEffects type;

	double damage, durationTime, maxRadiusSize;
	int howManyTimesItHits;
	sf::CircleShape circle;
	sf::Sprite		sprite;

	// funkcja która zarz¹dza animacj¹
	double (*func)(double,int);
	double animCounter;
	double timeOfFunc;
	double funcStep;

	bool needToDelete;

	sf::Clock animationClock;

};


class ProjectileTerrainEffect : public terrainEffect
{
public:
	// zwraca 1 je¿eli trzeba usun¹æ, 0 w innym wypadku
	virtual void create(terrainEffectParams &params, sf::Vector2f pos) override;
	virtual void update() override;



	// kastety 
	virtual void create(terrainEffectParams &params, std::vector<projectile> &projectileArray);

protected:
	double diffBtwNewAndOldCircle;

};