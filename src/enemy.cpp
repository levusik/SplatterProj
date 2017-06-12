#include "enemy.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ustawienie parametrów z Szablonu przeciwnika 
void splat::create(int groupIndex, splatTemplate splatUnit)
{
	// TODO (!)
	// podczas tworzenia orka normalnie bêdziemy losowaæ aczkolwiek teraz 
	// wybierzemy sobie jak¹œ wartoœæ 
	this->armor = getRandomVal(splatUnit.minArmor, splatUnit.maxArmor);
	this->Damage = getRandomVal(splatUnit.minDamage, splatUnit.maxDamage);
	this->groupIndex = groupIndex;
	this->HP = getRandomVal(splatUnit.minHP, splatUnit.maxHP);
	this->minSpeed = splatUnit.minSpeed;
	this->maxSpeed = splatUnit.maxSpeed;

	this->circle.setPosition(splatUnit.startingPosition);
	this->circle.setFillColor(splatUnit.passiveModeColor);
	this->circle.setRadius(splatUnit.radius);
	this->V.x = rand() % static_cast<int>(splatUnit.minSpeed) - rand() % static_cast<int>(splatUnit.maxSpeed);
	this->V.y = rand() % static_cast<int>(splatUnit.minSpeed) - rand() % static_cast<int>(splatUnit.maxSpeed);
	this->minDistanceFromMates = 4 * (circle.getRadius());

	this->sizeOfView = splatUnit.sizeOfView;
	this->angleOfView = splatUnit.angleOfView;
	this->pith = splatUnit.pith;

	// kolorki
	this->agressiveModeColor = splatUnit.agressiveModeColor;
	this->passiveModeColor = splatUnit.passiveModeColor;
	this->ChargingColor = splatUnit.ChargingColor;
	this->attackingPlayerColor = splatUnit.attackingPlayerColor;
	this->fleeingColor = splatUnit.fleeingColor;
	this->reactingtoPlayerColor = splatUnit.reactingtoPlayerColor;


	// pocz¹tkowy stan AI czyli szwendanie siê 
	this->currentState = stateOfEnemy::WALKING_ARROUND;

	this->changeMoveBy = splatUnit.changeMoveBy;
	this->timeToMakeDecisionInSec = splatUnit.timeToMakeDecisionInSec;


	// zmienne potrzebne do ruchu w kierunku gracza
	this->movementToPlayerX = splatUnit.minSpeed;
	this->movementToPlayerY = splatUnit.minSpeed;
	this->sinA = 0;
	this->cosA = 0;

	this->acceleration = splatUnit.acceleration;
	this->timeOfDeceleration = 0.15;
	this->chargeDistance = splatUnit.chargeDistance + this->circle.getRadius();


	// zmienna która wyra¿a dystans do którego kreatura d¹¿y	(promieñ gracza + promieñ kreatury + zasiêg broni)

	this->safeDistance = splatUnit.safeDistance;


	this->slowDistance = safeDistance + 50;

	this->hasCharged = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


void splat::update(Verticle &averageV, double &averageDistance, Verticle playerPos)
{
	// co by siê nie dzia³o to zawsze sobie liczymy dystans kreatury od gracza 
	distanceBtwCreatureAndPlayer = calculateDistance(this->getCenter(), playerPos);

	this->average = averageV;
	this->averageDistance = averageDistance;

	// FSM Przeciwnika
	switch (currentState)
	{
	case stateOfEnemy::WALKING_ARROUND:
		walkingArround(playerPos);
		manageVertexLine(this->V.x, this->V.y, this->sinA, this->cosA);
		break;
	case stateOfEnemy::REACT_TO_PLAYER:
		reactToPlayer(playerPos);
		break;
	case stateOfEnemy::ATTACK_PLAYER:
		attackPlayer();
		break;
	case stateOfEnemy::MOVING_TOWARDS_PLAYER:
		movingTowardsPlayer(playerPos);
		manageVertexLine(std::abs(this->movementToPlayerX), std::abs(this->movementToPlayerY), this->sinA, this->cosA);
		break;
	}
}



/////////////////////////////////////////////////////////////////////
//	Pobranie sobie parametrów orków tej samej grupy (tylko kilku)
void splat::setParametersOfOtherMates(std::vector<otherMatesParameters>& ParametersOfOtherMates)
{
	this->parametersOfOtherMates = ParametersOfOtherMates;
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//		Je¿eli ta metoda zosta³a wywo³ana to znaczy 
//		¿e stan gry wykry³ ¿e jakiœ pocisk zetkn¹³ siê 
//		ze splatem (i podemujemy jakieœ akcje w zale¿noœci
//		od parametrów kulki)
void splat::dealDamage(bulletParameters collidedBullet)
{
	this->HP -= collidedBullet.damage;

	if (HP <= 0)
		this->isAlive = false;
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	Szwêdanie siê orka
void splat::walkingArround(Verticle playerPos)
{
	//	 wylosowanie kierunku ruchu co jakiœ czas 
	//   oraz update pozycji 
	if (GETSEC(delayBtwChangingMove) >= this->changeMoveBy / 2)
	{
		double randX = rand() % 6 - rand() % 4;
		this->V.x = randX;

		double randY = rand() % 6 - rand() % 4;
		this->V.y = randY;
		delayBtwChangingMove.restart();
	}

	// sprawdzamy czy gracz jest w zasiêgu wzroku
	if (distanceBtwCreatureAndPlayer < this->sizeOfView || this->playerDetected)
	{

		this->playerDetected = true;

		// wykryto bohatera zatem orki zaczynaj¹ atakowaæ 
		this->circle.setFillColor(reactingtoPlayerColor);

		//zmieniamy stan splata 
		this->currentState = stateOfEnemy::REACT_TO_PLAYER;
		timeToMakeDecision.restart();
		return;
	}
	// warunek pierwszy : a.k.a ka¿dy boid dopasowuje swoj¹ prêdkoœæ do reszty stada
	V.x += apply1Rule(average.x, V.x, this->pith);
	V.y += apply1Rule(average.y, V.y, this->pith);

	for (int i = 0; i < this->parametersOfOtherMates.size(); ++i)
	{
		// je¿eli nie wykryto wroga w ca³ej grupie to po prostu boid siê szwenda 
		if (!parametersOfOtherMates[i].hasDetectedPlayer)
		{
			double distance = calculateDistance(this->getCenter(), parametersOfOtherMates[i].position);
			V.x += apply2Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->pith);
			V.y += apply2Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->pith);

			// sprawdzamy czy ziomek-boid nie zbli¿y³ siê nazbyt do aktualnego boida
			if (distance < this->minDistanceFromMates)
			{
				V.x += apply3Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->pith);
				V.y += apply3Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->pith);
			}
		}
		// natomiast je¿eli wykryto wroga to ca³a grupa kmini co zrobiæ ( w sensie ¿e reaguje na gracza)
		else
		{
			this->playerDetected = true;

			// wykryto bohatera zatem orki zaczynaj¹ atakowaæ 
			this->circle.setFillColor(reactingtoPlayerColor);
			this->currentState = stateOfEnemy::REACT_TO_PLAYER;
			return;
		}
	}

	manageVertexLine(V.x, V.y, 1, 1);

	circle.move(V.x, V.y);
	this->move(V.x, V.y);
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	 Reagowanie na gracza
void splat::reactToPlayer(Verticle playerPos)
{
	// kreatury dopiero po pewnym czasie podejmuj¹ decyzjê 
	if (timeToMakeDecision.getElapsedTime().asSeconds() > this->timeToMakeDecisionInSec)
	{

		movementToPlayerX = V.x;
		movementToPlayerY = V.y;

		// ustawiamy stan na pod¹¿anie w kierunku bohatera
		this->currentState = stateOfEnemy::MOVING_TOWARDS_PLAYER;
		this->circle.setFillColor(this->agressiveModeColor);

	}
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	Ruch w kierunku gracza
void splat::movingTowardsPlayer(Verticle playerPos)
{
	if (this->distanceBtwCreatureAndPlayer > this->safeDistance)
	{

		// liczymy sobie funkcje trygonometryczne
		double distBtwPlayerAndCreature, distanceX, distanceY;
		distanceX = playerPos.x - this->getCenter().x;
		distanceY = playerPos.y - this->getCenter().y;
		sinA = distanceX / distanceBtwCreatureAndPlayer;
		cosA = distanceY / distanceBtwCreatureAndPlayer;

		// sprawdzamy czy szar¿a jest mo¿liwa 
		charge();

		// zarz¹dzamy przyspieszeniem oraz regu³y algorytmu boidów
		manageAcceleration();

		double moveX = std::abs(movementToPlayerX) * sinA;
		double moveY = std::abs(movementToPlayerY) * cosA;


		//this->circle.move(moveX,0);

		// update'ujemy nasze kó³ko
		this->circle.move(moveX, moveY);
	}
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Szar¿a 
void splat::charge()
{
	if (distanceBtwCreatureAndPlayer < this->chargeDistance && !hasCharged)
	{
		hasCharged = true;
		this->circle.setFillColor(ChargingColor);
		acceleration *= 2;
	}
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	Atak gracza
void splat::attackPlayer()
{
}
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
//	przyspieszenie gracza 
void splat::manageAcceleration()
{

	if (this->distanceBtwCreatureAndPlayer > this->slowDistance && std::abs(this->movementToPlayerX) < std::abs(this->maxSpeed) && std::abs(movementToPlayerY) < this->maxSpeed)
	{
		// metoda zarz¹dzaj¹ca przyspieszeniem
		if (sinA >= 0)
		{
			this->movementToPlayerX += this->acceleration;
		}
		else
		{
			this->movementToPlayerX -= this->acceleration;
		}
		if (cosA >= 0)
		{
			this->movementToPlayerY += this->acceleration;
		}
		else
		{
			this->movementToPlayerY -= this->acceleration;
		}
	}
	else
	{
		movementToPlayerX -= movementToPlayerX / FPS;
		movementToPlayerY -= movementToPlayerY / FPS;
	}
	if (std::abs(movementToPlayerX) < this->maxSpeed && std::abs(movementToPlayerY) < this->maxSpeed)
	{
		//wprowadŸmy trochê chaosu w ruchu splatów 
		movementToPlayerX += rand() % 1 - rand() % 2;
		movementToPlayerY += rand() % 1 - rand() % 2;


		// stosujemy 1 zasadê algorytmu boidów a.k.a. prêdkoœæ ca³ej grupy ma wp³yw na prêdkoœæ ca³ej grupy 
		movementToPlayerX += apply1Rule(this->average.x, this->movementToPlayerX, this->pith);
		movementToPlayerY += apply1Rule(this->average.y, this->movementToPlayerY, this->pith);



		// stosujemy te¿ 2 i 3 zasadê algorytmu boidów 
		for (int i = 0; i < parametersOfOtherMates.size(); ++i)
		{
			// stosujemy 2 regulê algorytmu
			double distance = calculateDistance(this->getCenter(), parametersOfOtherMates[i].position);
			movementToPlayerX += apply2Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->pith);
			movementToPlayerY += apply2Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->pith);

			// sprawdzamy czy przyjaciel boida nie zbli¿y³ siê nazbyt do aktualnego boida
			if (distance < this->minDistanceFromMates)
			{
				double add;
				movementToPlayerX += apply3Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, 0.2);
				movementToPlayerY += apply3Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, 0.2);
			}
		}
	}

}
/////////////////////////////////////////////////////////////////////



// regu³a 2 : wzór
//	odl =  sqrt((s.x - b.x)**2 + (s.y - b.y)**2)
//	b.vx = b.vx + waga *(s.x - b.x) *(odl - avgOdl) / odl
//	b.vy = b.vy + waga *(s.y - b.y) *(odl - avgOdl) / odl
//	

/////////////////////////////////////////////////////////////////////
