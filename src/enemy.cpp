#include "enemy.hpp"

/******************************************************************************************************************/
void splat::setParametersOfOtherMates(std::vector<otherMatesParameters>& ParametersOfOtherMates)
{
	this->parametersOfOtherMates = ParametersOfOtherMates;
}

/******************************************************************************************************************/
void splat::dealDamage(damageParameters damage)
{
	this->HP -= damage.value;

	this->reactToGettingHit();
}

/******************************************************************************************************************/
void splat::create(int groupIndex, splatTemplate enemyTemplate)
{

	// TODO
	this->armor = getRandomVal(enemyTemplate.minArmor, enemyTemplate.maxArmor);
	this->Damage = getRandomVal(enemyTemplate.minDamage, enemyTemplate.maxDamage);
	this->groupIndex = groupIndex;
	this->HP = getRandomVal(enemyTemplate.minHP, enemyTemplate.maxHP)* 100;
	this->minSpeed = enemyTemplate.minSpeed;
	this->maxSpeed = enemyTemplate.maxSpeed;


	this->circle.setPosition(enemyTemplate.startingPosition);
	this->circle.setFillColor(enemyTemplate.passiveModeColor);
	this->circle.setRadius(enemyTemplate.radius);
	this->V.x = rand() % 4 - rand() % 4;
	this->V.y = rand() % 4 - rand() % 4;
	this->minDistanceFromMates = 4 * (circle.getRadius());


	this->sizeOfView = enemyTemplate.sizeOfView;
	this->angleOfView = enemyTemplate.angleOfView;
	this->minDistanceFromPlayer = 300;

	//
	//this->pith = enemyTemplate.pith;
	this->pith = 0.2;
	this->speedBuff  =  1.;
	this->attackBuff =  1.;
	this->rangeOfWeapon = 100.;
	this->isInFightingState = false;
	this->delayBtwAttacksInSecs = 1.;

	//


	// HARDCODED  (!) {zrobiæ odzczyt z pliku}
	// kolorki
	this->colorOfHPBar				= sf::Color::Color(0, 0xff, 0xff);
	this->IdleStateColor			= sf::Color::Color(0x46, 0x3e, 0xee);
	this->IntriguedStateColor		= sf::Color::Color(0xee, 0xee, 0x0);
	this->makingDecisionStateColor	= sf::Color::Color(0xff, 0xff, 0x0);
	this->agressiveStateColor		= sf::Color::Color(0xff, 0x0, 0x0);
	this->chargeStateColor			= sf::Color::Color(0xff, 0x7f, 0x0);
	this->attackStateColor			= sf::Color::Color(0xee, 0x0, 0x0);
	this->fleeStateColor			= sf::Color::Color(0x9b, 0x30, 0xff);
	this->specialAbilityStateColor	= sf::Color::Color(0x85, 0x63, 0x63);

	// pocz¹tkowy stan AI czyli szwendanie siê 
	this->currentState = stateOfEnemy::IDLE;
	this->circle.setFillColor(this->IdleStateColor);


	this->changeMoveBy = enemyTemplate.changeMoveBy;
	this->timeToMakeDecisionInSec = enemyTemplate.timeToMakeDecisionInSec;


	// zmienne potrzebne do ruchu w kierunku gracza
	this->sinA = 0;
	this->cosA = 0;
	this->isGroupAgressive = 0;
	this->isGroupIntrigued = 0;


	this->acceleration = enemyTemplate.acceleration;
	this->timeOfDeceleration = 0.15;
	this->chargeDistance = enemyTemplate.chargeDistance + this->circle.getRadius();


	// zmienna która wyra¿a dystans do którego kreatura d¹¿y	(promieñ gracza + promieñ kreatury + zasiêg broni)

	this->safeDistance = enemyTemplate.safeDistance;


	this->slowDistance = safeDistance + 50;

	this->hasCharged = false;


	// utworzenie HPBara 
	this->HPRectangle.init(this->HP, 2*this->circle.getRadius(), 0.5 * this->circle.getRadius(), colorOfHPBar);
	

	this->dmgParameters.value = this->Damage;
}

/******************************************************************************************************************/
//		Schemat AI rozpisany w zeszycie
void splat::manageAIScheme(sf::Vector2f *averageV, double *averageDistance, player *Player)
{
	this->playerPointer		= Player;
	this->averageDistance	= averageDistance;
	this->averageV			= averageV;

	this->distanceBtwCreatureAndPlayer = calculateDistance(Player->getCenter(), this->getCenter());
	
	switch (this->currentState)
	{
	case stateOfEnemy::IDLE:
	{
		this->IDLEState();
		break;
	}
	case stateOfEnemy::INTRIGUED:
	{
		this->intriguedState();
		break;
	}
	case stateOfEnemy::MAKINGDECISION:
	{
		this->makingDecisionState();
		break;
	}
	case stateOfEnemy::AGRESSIVE:
	{
		this->agressiveState();
		break;
	}
	case stateOfEnemy::ATTACK:
	{
		this->attackState();
		break;
	}
	case stateOfEnemy::CHARGE:
	{
		this->chargeState();
		break;
	}
	case stateOfEnemy::SPECIALABILITY:
	{
		this->specialAbility();
		break;
	}
	case stateOfEnemy::FLEE:
	{
		this->fleeState();
		break;
	}
	}


	if (this->movementDelay.getElapsedTime().asSeconds() > 1./FPS && V.x != 0 && V.y != 0 )
	{
		this->handleBoundingRect();
		this->circle.move(V.x, V.y);
		this->movementDelay.restart();
	}
}

/******************************************************************************************************************/
void splat::IDLEState()
{
	// wywo³anie metody która zarz¹dza przejœciami do innych stanów
	this->IDLESChangeToDiffState();
	
	// losujemy sobie kierunek ruchu ( wprowadzenie lekkiej dawki chaosu)
	if (this->delayBtwRandomizeMovement.getElapsedTime().asSeconds() + timeAdd > 3.f)
	{
		randomizeMovement();
	}
	
	// update zasad algorytmu boidów 
	if (delayBtwBoidAlgorithm.getElapsedTime().asSeconds() > 1. / 30.)
	{
		// stosujemy wszystkie regu³y algorytmu boidów	
		this->V += enemy::applyAllRules(*this->averageV, *this->averageDistance);
		this->delayBtwBoidAlgorithm.restart();
	}
	
	// ograniczenie wektora
	this->limitVector();

}

/******************************************************************************************************************/
void splat::IDLESChangeToDiffState()
{
	
	// przjeœcie w stan Intrigued 
	//if (calculateDistance(Player.getCenter(), this->getCenter()) <= this->sizeOfView || this->isGroupIntrigued)
	if (calculateDistance(this->playerPointer->getCenter(), this->getCenter()) <= this->minDistanceFromPlayer)
	{
		this->circle.setFillColor(this->IntriguedStateColor);
		this->isGroupIntrigued = true;
		this->currentState		= stateOfEnemy::INTRIGUED;
	}

	if (this->playerHurtedGroupMate)
	{
		this->circle.setFillColor(this->agressiveStateColor);
		this->currentState = stateOfEnemy::AGRESSIVE;
	}

}

/******************************************************************************************************************/
void splat::intriguedState()
{

	this->intriguedSChangeToDiffState();

	applyAllRules(*this->averageV, *this->averageDistance);

	// gdy jest zaintrygowany, porusza siê w stronê gracza
	this->calculateTrygFunc();

	V.x = this->minSpeed * this->cosA;
	V.y = this->minSpeed * this->sinA;
}

/******************************************************************************************************************/
void splat::intriguedSChangeToDiffState()
{
	if (this->playerHurtedGroupMate)
	{
		this->circle.setFillColor(this->agressiveStateColor);
		this->currentState = stateOfEnemy::AGRESSIVE;
	}

	if (this->distanceBtwCreatureAndPlayer >= this->minDistanceFromPlayer)
	{
		this->currentState = stateOfEnemy::IDLE;
		this->circle.setFillColor(this->IdleStateColor);
	}
	

	if (this->distanceBtwCreatureAndPlayer <= this->minDistanceFromPlayer/1.5)
	{
		this->currentState = stateOfEnemy::MAKINGDECISION;
		this->circle.setFillColor(this->makingDecisionStateColor);
		this->makingDecisionDelay.restart();
	}
}

/******************************************************************************************************************/
void splat::makingDecisionState()
{
	this->makingDecisionChangeToDiffState();


	V.x = 0;
	V.y = 0;
}

/******************************************************************************************************************/
void splat::makingDecisionChangeToDiffState()
{
	if (this->makingDecisionDelay.getElapsedTime().asSeconds() > timeToMakeDecisionInSec)
	{
		if (this->isWorthAttacking())
		{
			this->currentState = stateOfEnemy::AGRESSIVE;
			this->circle.setFillColor(this->agressiveStateColor);
		}
		else
		{
			this->currentState = stateOfEnemy::FLEE;
			this->circle.setFillColor(this->fleeStateColor);
		}
	}
}

/******************************************************************************************************************/
void splat::fleeState()
{
	this->fleeChangeToDiffState();

	// policzenie funkcji trygonometrycznych
	this->calculateTrygFunc();

	// obliczenie vektora ruchu
	this->V.x = this->cosA * this->maxSpeed * -1;
	this->V.y = this->sinA * this->maxSpeed * -1;
}

/******************************************************************************************************************/
void splat::fleeChangeToDiffState()
{
	if (this->distanceBtwCreatureAndPlayer >= this->minDistanceFromPlayer* fleeDistancePrc)
	{
		this->currentState = stateOfEnemy::IDLE;
		this->circle.setFillColor(this->IdleStateColor);
	}

}

/******************************************************************************************************************/

/******************************************************************************************************************/
/*											SYSTEM WALKI														  */
/******************************************************************************************************************/
void splat::agressiveState()
{
	this->agressiveChangeToDiffState();

	if (this->distanceBtwCreatureAndPlayer >= 2 * this->circle.getRadius() + this->playerPointer->getCircle().getRadius())
	{
		this->calculateTrygFunc();
		this->V.x = this->maxSpeed * this->cosA * this->speedBuff;
		this->V.y = this->maxSpeed * this->sinA * this->speedBuff;
	}
	else
	{
		this->V.x = 0;
		this->V.y = 0;
	}

}
/******************************************************************************************************************/
void splat::agressiveChangeToDiffState()
{
	if (this->distanceBtwCreatureAndPlayer <= this->chargeDistance && !hasCharged)
	{
		this->currentState = stateOfEnemy::CHARGE;
		this->circle.setFillColor(this->chargeStateColor);
	}

	if (this->distanceBtwCreatureAndPlayer <= this->rangeOfWeapon)
	{
		this->currentState = stateOfEnemy::ATTACK;
		this->circle.setFillColor(this->attackStateColor);
		this->timeToMakeDecision.restart();
	}
}
/******************************************************************************************************************/
void splat::chargeState()
{
	chargeChangeToDiffState();
	this->speedBuff = 1.2f;
	this->hasCharged = true;
}
/******************************************************************************************************************/
void splat::chargeChangeToDiffState()
{
	this->currentState = stateOfEnemy::AGRESSIVE;
	this->circle.setFillColor(this->agressiveStateColor);
}
/******************************************************************************************************************/
void splat::attackState()
{
	this->attackChangeToDiffState();

	if (this->distanceBtwCreatureAndPlayer > this->playerPointer->getCircle().getRadius() + 1.3*this->getCircle().getRadius())
	{
		calculateTrygFunc();
		this->V.x = this->maxSpeed * this->cosA * this->speedBuff;
		this->V.y = this->maxSpeed * this->sinA * this->speedBuff;
	}
	else
	{
		V.x = 0;
		V.y = 0;
	
		if (this->distanceBtwCreatureAndPlayer < this->circle.getRadius() + playerPointer->getCircle().getRadius())
		{
			calculateTrygFunc();
			V.x = (distanceBtwCreatureAndPlayer - playerPointer->getCircle().getRadius() - circle.getRadius())* cosA;
			V.y = (distanceBtwCreatureAndPlayer - playerPointer->getCircle().getRadius() - circle.getRadius()) * sinA;
		}
	}

	if (GETSEC(delayBtwAttacksClock) > this->delayBtwAttacksInSecs)
	{
		this->playerPointer->dealDamage(this->dmgParameters);
		this->delayBtwAttacksClock.restart();
	}
	

}
/******************************************************************************************************************/
void splat::attackChangeToDiffState()
{
	if (this->distanceBtwCreatureAndPlayer >= getCircle().getRadius() + this->playerPointer->getCircle().getRadius() + rangeOfWeapon)
	{
		isInFightingState = false;
		this->circle.setFillColor(agressiveStateColor);
		this->currentState = stateOfEnemy::AGRESSIVE;
	}
}
/******************************************************************************************************************/
void splat::specialAbility()
{
}
/******************************************************************************************************************/
void splat::specialAbilityChangeToDiffState()
{
}
/******************************************************************************************************************/
bool splat::isWorthAttacking()
{
	return false;
}
/******************************************************************************************************************/
void splat::reactToGettingHit()
{

	this->gettingHitChangeStates();
	

	// sprawdzamy poziom zdrowia przeciwnika 
	if (this->HP <= 0)
	{
		this->isAlive = false;
	}
}
/******************************************************************************************************************/
void splat::gettingHitChangeStates()
{
	if (this->currentState != stateOfEnemy::AGRESSIVE && this->currentState != stateOfEnemy::CHARGE &&
		this->currentState != stateOfEnemy::SPECIALABILITY && this->currentState != stateOfEnemy::ATTACK)
	{
		this->currentState = stateOfEnemy::AGRESSIVE;
		this->circle.setFillColor(this->agressiveStateColor);
	}
}
/******************************************************************************************************************/
void splat::randomizeMovement()
{
		int avg = (this->maxSpeed + this->minSpeed) / 2;
		this->V.x = rand() % avg - rand() % avg;
		this->V.y = rand() % avg - rand() % avg;
		this->delayBtwRandomizeMovement.restart();
		timeAdd = 0.f;


		if (VxMustBePlus)
		{
			V.x = std::abs(V.x);
			VxMustBePlus = false;

		}
		else if (VxMustBeNeg)
		{
			V.x = -std::abs(V.x);
			VxMustBeNeg = false;
		}
		if (VyMusBePlus)
		{
			V.y = std::abs(V.y);
			VyMusBePlus = false;
		}
		else if (VyMusBeNeg)
		{
			V.y = -std::abs(V.y);
			VyMusBeNeg = false;
		}
}
/******************************************************************************************************************/
