#pragma once

// agresywny przeciwnik dziedziczy wi�kszo�� zachowa� po tch�rzliwym przeciwniku 
class agressiveEnemy : public flightyEnemy
{
public:
	agressiveEnemy()
	{
		this->colorOfHPBar		= sf::Color::Color(0x99,0x32,0xcc);
		groupAnimal = false;
		directionX = directionY = 0.;
		movememtLock = false;
	}
protected:
	// w przeciwnie�stwie do tch�rzliwego przeciwnika, agresywny przechodzi w tryb polowania 
	// (porusza si� mniej wi�cej w kierunku gracza)
	virtual void normalMovement()
	{
		calculateTrygFunc();
		this->moveX	= this->parameters.maxSpeed.x  * directionX;
		this->moveY	= this->parameters.maxSpeed.y  * directionY;

	}

	// losuje kierunki te� mniej wi�cej w stron� gracza 
	virtual void randomizeMovement()
	{
		calculateTrygFunc();
		// chcemy by przeciwnik porusza� si� mniej wi�cej w stron� gracza 
		directionX = double(rand() % 400) / 1000. - double(rand() % 400) / 1000.;
		directionY = double(rand() % 400) / 1000. - double(rand() % 400) / 1000.;
		this->directionX += cosA;
		this->directionY += sinA;
	}
	// zawzwyczaj dla niego jest warto atakowa� 
	virtual bool isWorthAttacking()
	{
		return true;
	}
	
	double directionX, directionY;
};