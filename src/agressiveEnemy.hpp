#pragma once

// agresywny przeciwnik dziedziczy wiêkszoœæ zachowañ po tchórzliwym przeciwniku 
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
	// w przeciwnieñstwie do tchórzliwego przeciwnika, agresywny przechodzi w tryb polowania 
	// (porusza siê mniej wiêcej w kierunku gracza)
	virtual void normalMovement()
	{
		calculateTrygFunc();
		this->moveX	= this->parameters.maxSpeed.x  * directionX;
		this->moveY	= this->parameters.maxSpeed.y  * directionY;

	}

	// losuje kierunki te¿ mniej wiêcej w stronê gracza 
	virtual void randomizeMovement()
	{
		calculateTrygFunc();
		// chcemy by przeciwnik porusza³ siê mniej wiêcej w stronê gracza 
		directionX = double(rand() % 400) / 1000. - double(rand() % 400) / 1000.;
		directionY = double(rand() % 400) / 1000. - double(rand() % 400) / 1000.;
		this->directionX += cosA;
		this->directionY += sinA;
	}
	// zawzwyczaj dla niego jest warto atakowaæ 
	virtual bool isWorthAttacking()
	{
		return true;
	}
	
	double directionX, directionY;
};