#pragma once


class bitter : public runner
{
public:
	bitter()
	{

	}

protected:
	// wyj¹tkowo bezsensowne by overridowaæ ca³¹ metodê [ (!) REFAKTORYZACJA ]
	virtual void	manageMovement() override
	{

		this->V.x += this->acceleration.x * getSign(this->V.x);
		this->V.y += this->acceleration.y * getSign(this->V.y);
		limitVector();

		// co by siê nie dzia³o prawa fizyki pozostaj¹ te same
		if (!movememtLock)
		{

			if (runAway)
				runAwayFromPlayer();

			else if (moveToPlayer)
					moveTowardsPlayer();
			else
				normalMovement();


			movememtLock = false;

			time = GETSEC(clock);

			handleBoundingRect();


			this->circle.move(moveX * GETSEC(clock), moveY * GETSEC(clock));
			manageVertexLine();
		}

		clock.restart();
	}

	virtual void moveTowardsPlayer() override
	{
		this->V.x = abs(this->parameters.maxSpeed.x) * this->cosA;
		this->V.y = abs(this->parameters.maxSpeed.x) * this->sinA;
		moveToPlayer = false;
		moveX = moveY = 0;

		if (GETSEC(generatingNewRadiusClock) > 0.5f)
		{
			multipler *= 0.99f;
			generatingNewRadiusClock.restart();
		}


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
		
		this->moveX  = this->moveY = 0;
		radPlayerToEnemy = atan2(this->getCenter().y - this->playerPointer->getCenter().y,
								 this->getCenter().x - this->playerPointer->getCenter().x);
		this->V.x = this->V.y = 0;
		
		if (!this->calculatingRadiansLock)
		{
			this->actualRadius = this->distanceBtwCreatureAndPlayer;
			calculatingRadiansLock = true;
		}

		this->circle.setPosition(this->playerPointer->getCenter().x + cos(this->radPlayerToEnemy + this->cosValAdd ) *  actualRadius * multipler ,
								 this->playerPointer->getCenter().y + sin(this->radPlayerToEnemy + this->sinValAdd ) *  actualRadius * multipler );

		cosValAdd += GETSEC(timeOfFrame) * M_PI / 180.;
		sinValAdd += GETSEC(timeOfFrame) * M_PI / 180.;
		timeOfFrame.restart();

	}
	virtual void changeToAgressiveState() override
	{
		enemy::changeToAgressiveState();
		calculateTrygFunc();
		this->restartClock = true;
		calculatingRadiansLock = false;
	}

	sf::Clock generatingNewRadiusClock, timeOfFrame;
	double actualRadius = 0, subtract = 0;
	double radPlayerToEnemy;
	double multipler = 1.;
	double cosValAdd = 0, sinValAdd = 0;
	bool   restartClock = false;
	bool   calculatingRadiansLock = false;
};