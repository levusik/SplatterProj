#pragma once

#ifndef ENEMY_H_ADDED
#include "enemy.hpp"

#endif 


class flightyEnemy : public enemy
{
public:
	flightyEnemy()
	{
		groupAnimal = true;
		percOfMaxHPWhenEnemyFlee = 0.4f;
	}
protected:

	/*==========================================================================================================*/
	virtual void IDLEState()
	{
		if (GETSEC(this->delayBtwRandomizeMovement) + timeAdd > 12.f)
			randomizeMovement();

		// w stanie IDLE na ruch ma wp³yw algorytm boidów
		V += applyAllRules(*this->averageV, *this->averageDistance);

		// wywo³anie metody która zajmie siê przejœciami do innych stanów
		this->IDLESChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::IDLESChangeToDiffState()
	{
		if (distanceBtwCreatureAndPlayer <= this->parameters.safeDistance)
		{
			this->circle.setFillColor(IntriguedStateColor);
			this->currentState = stateOfEnemy::INTRIGUED;
			if (distanceBtwCreatureAndPlayer <= this->parameters.miniumDistance)
				this->changeToAgressiveState();
		}

		if (this->playerHurtedGroupMate && groupAnimal)
			this->changeToAgressiveState();
	}

	/*==========================================================================================================*/
	virtual void intriguedState()
	{
		// gdy jest zaintrygowany porusza siê w stronê gracza
		this->calculateTrygFunc();
		this->moveToPlayer = true;

		V.x += parameters.maxSpeed.x / 4.;
		V.y += parameters.maxSpeed.y / 4.;

		intriguedSChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::intriguedSChangeToDiffState()
	{
		if (this->playerHurtedGroupMate)
			this->changeToAgressiveState();

		if (this->distanceBtwCreatureAndPlayer <= this->parameters.viewSize)
		{
			this->currentState = stateOfEnemy::MAKINGDECISION;
			this->circle.setFillColor(this->makingDecisionStateColor);
			this->makingDecisionTime.restart();
		}

	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::makingDecisionState()
	{
		makingDecisionChangeToDiffState();
		V.x = V.y = 0;
		movememtLock = true;
		if (GETSEC(this->makingDecisionTime) > parameters.reactionTime / 1000.)
		{
			if (this->isWorthAttacking())
				this->changeToAgressiveState();

			else
			{
				this->currentState = stateOfEnemy::FLEE;
				this->circle.setFillColor(this->fleeStateColor);
			}
		}
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::makingDecisionChangeToDiffState()
	{
		if (playerHurtedGroupMate)
		{
			if (groupAnimal)
				this->changeToAgressiveState();

			else if (this->ID == this->idOfHurtedEnemy)
			{
				// je¿eli ktoœ zosta³ zaatakowany 
				parameters.reactionTime = 0;
				this->idOfHurtedEnemy = -1;
			}
		}
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::fleeState()
	{
		this->V.x += this->parameters.maxSpeed.x  * getSign(this->V.x);
		this->V.y += this->parameters.maxSpeed.y  * getSign(this->V.y);

		calculateTrygFunc();
		runAway = true;
		this->moveToPlayer = true;

		this->fleeChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::fleeChangeToDiffState()
	{
		if (this->distanceBtwCreatureAndPlayer > this->parameters.safeDistance)
		{
			this->currentState = stateOfEnemy::IDLE;
			this->circle.setFillColor(this->IdleStateColor);
		}
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::agressiveState()
	{
		if (this->distanceBtwCreatureAndPlayer >= parameters.AttackRange + this->circle.getRadius() + this->playerPointer->getCircle().getRadius())
		{
			this->moveToPlayer = true;
			this->calculateTrygFunc();
			this->V.x = parameters.maxSpeed.x * speedBuff * getSign(this->V.x);
			this->V.y = parameters.maxSpeed.y * speedBuff* getSign(this->V.x);
		}
		agressiveChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::agressiveChangeToDiffState()
	{
		if (this->distanceBtwCreatureAndPlayer <= parameters.chargeDistance && !chargeLock)
		{
			this->currentState = stateOfEnemy::CHARGE;
			this->circle.setFillColor(this->chargeStateColor);
		}

		if (this->distanceBtwCreatureAndPlayer <= parameters.AttackRange + this->circle.getRadius() + this->playerPointer->getCircle().getRadius())
		{
			this->currentState = stateOfEnemy::ATTACK;
			this->circle.setFillColor(this->agressiveStateColor);
		}

	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::chargeState()
	{
		this->circle.setRadius(this->circle.getRadius() * enemyChargeRadiusBuff);
		this->speedBuff *= enemyChargeSpeedBuff;

		this->changeToAgressiveState();

		// chcemy by szar¿a zosta³a wywo³ana jednokrotnie 
		chargeLock = true;

		chargeChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::chargeChangeToDiffState()
	{
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::attackState()
	{
		calculateTrygFunc();

		moveToPlayer = true;
		V.x += this->parameters.maxSpeed.x * getSign(this->V.x) / 2.;
		V.y += this->parameters.maxSpeed.y * getSign(this->V.y) / 2.;
		attackChangeToDiffState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::attackChangeToDiffState()
	{
		if (!playerPointer->isAlive())
		{
			this->currentState = stateOfEnemy::IDLE;
			this->circle.setFillColor(this->IdleStateColor);
			this->circle.setFillColor(this->IdleStateColor);
		}
		if (this->HP < this->maxHP * percOfMaxHPWhenEnemyFlee && !fleeLock)
		{
			this->currentState = stateOfEnemy::FLEE;
			this->circle.setFillColor(this->fleeStateColor);
			fleeLock = true;
		}
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::specialAbility()
	{
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::specialAbilityChangeToDiffState()
	{
	}

	/*==========================================================================================================*/
	virtual bool flightyEnemy::isWorthAttacking()
	{
		return false;
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::reactToGettingHit()
	{
		this->gettingHitChangeStates();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::gettingHitChangeStates()
	{
		if (this->currentState != stateOfEnemy::AGRESSIVE &&
			this->currentState != stateOfEnemy::CHARGE    &&
			this->currentState != stateOfEnemy::ATTACK)
			this->changeToAgressiveState();
	}

	/*==========================================================================================================*/
	virtual void flightyEnemy::playAttackAnim()
	{
		//	 ?
	}

	/*==========================================================================================================*/
};
