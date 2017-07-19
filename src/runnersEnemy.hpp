#pragma once

// przeciwnik nastawiony na szybkie ataki
class runner : public agressiveEnemy
{
public:
	runner()
	{
		// du¿a szansa ¿e ucieknie 
		this->percOfMaxHPWhenEnemyFlee = .6f;
		this->colorOfHPBar = sf::Color::Color(0xee, 0xd8, 0xae);
		vecBuff = sf::Vector2f(0, 0);
		this->speedBuff = 2.f;
	}

protected:
	virtual void create(int groupIndex, splatTemplate enemyTemplate) override
	{
		enemy::create(groupIndex, enemyTemplate);
		origMaxSpeed	 = parameters.maxSpeed;
		origAcceleration = parameters.acceleration;
	}


	virtual void moveTowardsPlayer() override
	{
		
		this->V = this->vecBuff;
		this->V.x += this->parameters.acceleration.x * speedBuff * getSign(this->V.x);
		this->V.y += this->parameters.acceleration.y * speedBuff * getSign(this->V.y);
		this->vecBuff = this->V;
		
		if (this->vecBuff.x > this->parameters.maxSpeed.x)
			this->vecBuff.x *= 0.9;

		if (this->vecBuff.y > this->parameters.maxSpeed.y)
			this->vecBuff.y *= 0.9;


		moveX = abs(this->vecBuff.x) * (cosA );
		moveY = abs(this->vecBuff.y) * (sinA );


		for (int i = 0; i < this->parametersOfOtherMates.size(); ++i)
		{
			double distance = calculateDistance(this->getCenter(), parametersOfOtherMates[i].position);
			this->manageColliding(i, distance);

		}

		if (this->distanceBtwCreatureAndPlayer < this->playerPointer->getCircle().getRadius() + this->circle.getRadius())
		{
			double minDistance = this->playerPointer->getCircle().getRadius() + this->circle.getRadius() - distanceBtwCreatureAndPlayer + minDistanceConst;


			this->circle.move(minDistance * -cosA, minDistance * -sinA);

			V.x *= -0.75;
			V.y *= -0.75;
		}

	}
	
	virtual void agressiveChangeToDiffState() override
	{
		if (GETSEC(this->buffResetCounter) > 2.f + this->buffResetCounterTimeAdd && this->buffResetCounterTimeAdd < 4.)
		{
			this->chargeLock = false;
			this->buffResetCounter.restart();
		}

		if (this->distanceBtwCreatureAndPlayer <= parameters.chargeDistance && !chargeLock)
		{
			this->buffLock	   = false;
			this->currentState = stateOfEnemy::CHARGE;
			this->circle.setFillColor(this->chargeStateColor);
		}

		if (this->distanceBtwCreatureAndPlayer <= parameters.AttackRange + this->circle.getRadius() + this->playerPointer->getCircle().getRadius())
		{
			this->currentState = stateOfEnemy::ATTACK;
			this->circle.setFillColor(this->agressiveStateColor);
		}

	}
	virtual void chargeState() override
	{
		if (!buffLock)
		{
			this->parameters.acceleration.x  += this->origAcceleration.x;
			this->parameters.acceleration.y  += this->origAcceleration.y;
			this->parameters.maxSpeed.x      += this->origMaxSpeed.x;
			this->parameters.maxSpeed.x		 += this->origMaxSpeed.y;
			buffResetCounterTimeAdd += 1.f;
			
		
			this->buffLock = true;

		}

		this->chargeChangeToDiffState();
	}
	virtual void chargeChangeToDiffState() override
	{
		if (this->distanceBtwCreatureAndPlayer < this->parameters.AttackRange)
		{
			this->currentState = stateOfEnemy::ATTACK;
			this->circle.setFillColor(this->attackStateColor);
			chargeLock = true;
		}
		else
		{
			this->currentState = stateOfEnemy::AGRESSIVE;
			this->circle.setFillColor(this->agressiveStateColor);
			chargeLock = true;
		}
	}


	bool buffLock = false;
	sf::Vector2f vecBuff;
	double origRadiusSize, buffResetCounterTimeAdd;
	sf::Vector2f origAcceleration, origMaxSpeed;
	sf::Clock buffResetCounter;
};