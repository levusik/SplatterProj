#pragma once

// dziedziczy wiêkszoœæ zachowañ po tchórzliwym z kilkoma istotnymi zmianami 
// ( heavy jest skrajnie agresywny gdy siê  blisko podejdzie co objawia siê w buffach) 
class heavyEnemy : public flightyEnemy
{
public:
	heavyEnemy()
	{
		this->colorOfHPBar = sf::Color::Color(0x6f, 0x6f, 0x6f);
		percOfMaxHPWhenEnemyFlee = 0.f;		
	}

protected:
	virtual bool isWorthAttacking()
	{
		return true;
	}

	virtual void agressiveChangeToDiffState()
	{
		if (GETSEC(chargeDelay) > 2.f + this->circle.getRadius() / this->origCircle)
		{
			this->chargeLock = false;
			chargeDelay.restart();
		}

		if (this->distanceBtwCreatureAndPlayer <= parameters.chargeDistance && !chargeLock)
		{
			this->actCircle	= this->circle.getRadius();
			this->buffLock = false;
			this->animCounter = 0;
			this->currentState = stateOfEnemy::CHARGE;
			this->circle.setFillColor(this->chargeStateColor);
		}

		if (this->distanceBtwCreatureAndPlayer <= parameters.AttackRange + this->circle.getRadius() + this->playerPointer->getCircle().getRadius())
		{
			this->currentState = stateOfEnemy::ATTACK;
			this->circle.setFillColor(this->agressiveStateColor);
		}

	}
	virtual void intriguedSChangeToDiffState()
	{
		if (this->distanceBtwCreatureAndPlayer < this->parameters.miniumDistance)
		{
			this->currentState = stateOfEnemy::AGRESSIVE;
			this->circle.setFillColor(this->agressiveStateColor);
		}
	}
	virtual void chargeState()
	{
		this->calculateTrygFunc();
		this->moveToPlayer = true;

		if (!this->buffLock)
		{
			// heavy wpada w sza³ tzn. przywraca sobie do 75% utraconego zdrowia, powiêksza 
			// swój rozmiar, prêdkoœæ, zasiêg 
			this->HP = max(this->HP, 0.75f * this->parameters.maxHP);
			this->speedBuff = enemyChargeSpeedBuff * enemyChargeRadiusBuff;
			buffLock = true;
		}

		animCounter += 1.f;
		this->moveToPlayer = true;
		this->chargeChangeToDiffState();
	}
	virtual void chargeChangeToDiffState()
	{
		if (this->circle.getRadius() > this->actCircle * enemyChargeSpeedBuff || this->circle.getRadius() == this->origCircle * biggestPossibleCircleMultpiler)
		{
			if (this->distanceBtwCreatureAndPlayer < this->parameters.AttackRange)
			{
				this->currentState = stateOfEnemy::ATTACK;
				this->circle.setFillColor(this->attackStateColor);
				buffLock = false;
				chargeLock = true;
			}
			else
			{
				this->currentState = stateOfEnemy::AGRESSIVE;
				this->circle.setFillColor(this->agressiveStateColor);
				buffLock = false;
				chargeLock = true;
			}
		}
		else
		{
			double prevRadius = this->circle.getRadius();
			//																									najwiêksz¹ wartoœci¹ jest 
			//																									4 * pocz¹tkowy rozmiar kó³ka 
			double newRadius = min(this->circle.getRadius() + this->actCircle * abs(sin(animCounter * M_PI / 180.)), biggestPossibleCircleMultpiler * this->origCircle );
			this->circle.setRadius(newRadius);

			this->circle.move(prevRadius - newRadius, prevRadius - newRadius);
		}
	}
	virtual void create(int groupIndex, splatTemplate enemyTemplate)
	{
		enemy::create(groupIndex, enemyTemplate);
		this->origCircle = this->circle.getRadius();
	}

	const double biggestPossibleCircleMultpiler = 4.;
	double animCounter = 0;
	double buffLock = 0;
	double actCircle = 0, origCircle = 0;
	sf::Clock chargeDelay;
};