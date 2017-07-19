#include "enemy.hpp"


/***********************************************************************************/
// Konstruktor
enemy::enemy()
	: speedVector(sf::Lines, 8)
{
	timeAdd = 0;
	prevTime = 0;

	this->speedBuff = 1.f;
	this->attackBuff = 1.f;
	this->defenseBuff = 1.f;
	this->accelerationBuff = 1.f;
	this->percOfMaxHPWhenEnemyFlee = 0.2f;

	// kolorki !
	colorOfHPBar			 = sf::Color::Color(0, 0xff, 0);
	IdleStateColor			 = sf::Color::Color(0, 0xff, 0);
	IntriguedStateColor		 = sf::Color::Color(0, 0x20, 0xff);
	makingDecisionStateColor = sf::Color::Color(0x7f, 0x7f, 0);
	agressiveStateColor		 = sf::Color::Color(0xff, 0, 0);
	chargeStateColor		 = sf::Color::Color(0xff, 0x40, 0);
	attackStateColor		 = sf::Color::Color(0xA0, 0x10, 0x0);
	fleeStateColor			 = sf::Color::Color(0x50, 0, 0xbf);
	specialAbilityStateColor = sf::Color::Color(0xff, 0xff, 0xff);
	colorOfHPBar			 = sf::Color::Color(0x0, 0xff, 0x0);
	circleColor			     = sf::Color::Color(0, 0xff, 0x0);

	// 6 pierwszych wierzcho³ków to linie prêdkoœci
	for (int i = 0; i < 6; ++i)
		this->speedVector[i].color = sf::Color::Color(0xff, 0xff, 0);

	acceleration.x = acceleration.y = 0;

};

/***********************************************************************************/
void	enemy::setID(int id)
{
	this->ID = id;
}

/***********************************************************************************/
void	enemy::alertEnemy()
{
	this->playerHurtedGroupMate = true;
}

/***********************************************************************************/
void	enemy::intrigueEnemy()
{
	this->isGroupIntrigued = true;
}

/***********************************************************************************/
void	enemy::setDamageParameters(damageParameters newParams)
{
	this->dmgParameters = newParams;
}

/***********************************************************************************/
int		enemy::getGroupIndex()
{
	return this->groupIndex;
}

/***********************************************************************************/
int		enemy::getWorth()
{
	return this->parameters.worth;
}

/***********************************************************************************/
int		enemy::getID()
{
	return this->ID;
}

/***********************************************************************************/
bool	enemy::isIDLE()
{
	return this->currentState == stateOfEnemy::IDLE;
}

/***********************************************************************************/
bool	enemy::hasCreatureDetectedPlayer()
{
	return this->playerHurtedGroupMate;
}

/***********************************************************************************/
bool	enemy::isCreatureAlive()
{
	return  this->HP > 0 ? 1 : 0;
}

/***********************************************************************************/
bool	enemy::isEnemyIntrigued()
{
	return this->isGroupIntrigued;
}

/***********************************************************************************/
bool	enemy::canBeHittedByExplosion()
{
	return this->terrainDamageClock.getElapsedTime().asSeconds() > terrainDmgLock;
}

/***********************************************************************************/
double	enemy::getSizeOfView()
{
	return this->parameters.viewSize;
}

/***********************************************************************************/
double	enemy::getRandomVal(int min, int max)
{
	return rand() % (max - min) + min;
}

/***********************************************************************************/
double	enemy::getHP()
{
	return this->HP;
}

/***********************************************************************************/
sf::Vector2f		enemy::getCenter()
{
	// mo¿na to zmieœciæ w jednej linijce ale syntatic sugar zawsze na propsie
	sf::Vector2f position;
	position.x = this->circle.getPosition().x + this->circle.getRadius();
	position.y = this->circle.getPosition().y + this->circle.getRadius();
	return position;
}

/***********************************************************************************/
sf::Vector2f		enemy::getV()
{
	return this->V;
}

/***********************************************************************************/
sf::CircleShape		enemy::getCircle()
{
	return this->circle;
}

/***********************************************************************************/
sf::VertexArray		enemy::getVertex()
{
	return this->speedVector;
}

/***********************************************************************************/
HPbar				enemy::getHPBar() const
{
	return this->HPRectangle;
}

/***********************************************************************************/
void				enemy::create(int groupIndex, splatTemplate enemyTemplate)
{


	this->ID = groupIndex;
	this->parameters = enemyTemplate;
	this->HP = getRandomVal(parameters.minHP, parameters.maxHP);
	this->maxHP = HP;
	this->agility = getRandomVal(parameters.minAgility, parameters.maxAgility);
	this->armor = getRandomVal(parameters.minDefense, parameters.maxDefense);
	this->damage = getRandomVal(parameters.minDamage, parameters.maxDamage);

	this->circle.setRadius(parameters.circleRadius);
	this->circle.setFillColor(this->circleColor);
	this->circle.setPosition(enemyTemplate.startingPosition);

	this->parameters.minDistanceFromMates = 3 * circle.getRadius();

	this->V = sf::Vector2f(0, 0);

	this->currentState = stateOfEnemy::IDLE;
	this->circle.setFillColor(IdleStateColor);
	randomizeMovement();

	this->HPRectangle.init(this->maxHP, 2 * this->circle.getRadius(), 0.5f * this->circle.getRadius(), this->colorOfHPBar);
}

/***********************************************************************************/
void				enemy::update(sf::Vector2f *averageV, double *averageDistance, player *Player)
{
	// ustawienie naszych locków na domyœlne wartoœci
	this->movememtLock = false;
	this->moveToPlayer = false;


	// obliczamy sobie dystans
	distanceBtwCreatureAndPlayer = calculateDistance(Player->getCenter(), this->getCenter());

	// zapisujemy sobie wartoœci podane przez core
	this->averageV = averageV;
	this->averageDistance = averageDistance;
	this->playerPointer = Player;

	// FSM
	this->manageAIScheme();

	// update tego magicznego paseczka
	manageMovement();
	this->HPRectangle.update(this->getCenter(), this->HP, true, this->circle.getRadius());
}

/***********************************************************************************/
void				enemy::setParametersOfOtherMates(std::vector<otherMatesParameters> &ParametersOfOtherMates)
{
	this->parametersOfOtherMates = ParametersOfOtherMates;
}

/***********************************************************************************/
void				enemy::dealDamage(damageParameters Damage, int IDOfHurtedEnemy)
{
	this->HP -= Damage.value;

	idOfHurtedEnemy = IDOfHurtedEnemy;
	this->reactToGettingHit();
}

/***********************************************************************************/
void				enemy::manageAIScheme()
	{

		switch (currentState)
		{
		case stateOfEnemy::IDLE:
			IDLEState();
			break;
		case stateOfEnemy::INTRIGUED:
			intriguedState();
			break;
		case stateOfEnemy::MAKINGDECISION:
			makingDecisionState();
			break;
		case stateOfEnemy::FLEE:
			fleeState();
			break;
		case stateOfEnemy::AGRESSIVE:
			agressiveState();
			break;
		case stateOfEnemy::CHARGE:
			chargeState();
			break;
		case stateOfEnemy::ATTACK:
			attackState();
			break;
		case stateOfEnemy::SPECIALABILITY:
			specialAbility();
			break;

		}


	}

/***********************************************************************************/
void				enemy::manageMovement()
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

/***********************************************************************************/
void	enemy::moveTowardsPlayer()
{
	this->V.x = abs(this->parameters.maxSpeed.x) * this->cosA;
	this->V.y = abs(this->parameters.maxSpeed.x) * this->sinA;
	moveToPlayer = false;

	moveX = V.x;
	moveY = V.y;

	sf::Vector2f move = sf::Vector2f(moveX * time, moveY * time);
	
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

/***********************************************************************************/
void	enemy::runAwayFromPlayer()
{
	this->V = this->parameters.maxSpeed;
	moveX = abs(V.x) * cosA * -1;
	moveY = abs(V.y) * sinA * -1;

	runAway = false;
}

/***********************************************************************************/
void	enemy::normalMovement()
{
	this->moveX = V.x;
	this->moveY = V.y;
}

/***********************************************************************************/
void	enemy::randomizeMovement()
{
	randomAccelerationSign(acceleration.x);
	randomAccelerationSign(acceleration.y);
	delayBtwRandomizeMovement.restart();
}

/***********************************************************************************/
//	 metody potrzebne do algorytmu boidów oraz do update'u
void	enemy::manageVertexLine()
{
	this->speedVector[0].position.x = this->getCenter().x;
	this->speedVector[0].position.y = this->getCenter().y;

	this->speedVector[1].position.x = this->speedVector[0].position.x + V.x;
	this->speedVector[1].position.y = this->speedVector[0].position.y;

	this->speedVector[2] = this->speedVector[0];

	this->speedVector[3].position.x = this->speedVector[0].position.x;
	this->speedVector[3].position.y = this->speedVector[0].position.y + V.y;

	this->speedVector[4] = this->speedVector[0];

	this->speedVector[5].position.x = speedVector[1].position.x;
	this->speedVector[5].position.y = speedVector[3].position.y;

	this->speedVector[6] = speedVector[0];
	this->speedVector[7] = this->speedVector[6].position + this->acceleration;


}

/***********************************************************************************/
void	enemy::limitVector()
{
	if (std::abs(this->V.x) > this->parameters.maxSpeed.x *speedBuff)
		this->V.x *= 0.9;
	if (std::abs(this->V.y) > this->parameters.maxSpeed.y*speedBuff)
		this->V.y *= 0.9;

}

/***********************************************************************************/
void	enemy::calculateTrygFunc()
{
	if (distanceBtwCreatureAndPlayer != 0)
	{
		this->sinA = (this->playerPointer->getCenter().y - this->getCenter().y) / this->distanceBtwCreatureAndPlayer;
		this->cosA = (this->playerPointer->getCenter().x - this->getCenter().x) / this->distanceBtwCreatureAndPlayer;
	}
	else
	{
		this->sinA = 0;
		this->cosA = 0;
	}
}

/***********************************************************************************/
void	enemy::handleBoundingRect()
{
	// chcemy by splaty nie wychodzi³y poza okreœlony obszar
	if (this->circle.getPosition().x + moveX * time < LEFTWALL)
	{
		//		korzystamy z tego ¿e moveX musi byæ ujemne
		this->circle.setPosition(LEFTWALL + 2, this->circle.getPosition().y);
		this->V.x = abs(V.x) * 0.8;
		moveX = 0;
	}
	else if (this->circle.getPosition().x + this->V.x * time + 2 * this->circle.getRadius() > RIGHTWALL)
	{
		this->circle.setPosition((RIGHTWALL - 2 * this->circle.getRadius() - 2), this->circle.getPosition().y);
		this->V.x = -abs(V.x) * 0.8;
		moveX = 0;
	}
	if (this->circle.getPosition().y + this->V.y * time < UPWALL)
	{
		this->circle.setPosition(circle.getPosition().x, UPWALL + 2);
		this->V.y = abs(V.y) * 0.8;
		moveY = 0;
	}
	else if (this->circle.getPosition().y + 2 * this->circle.getRadius() + this->V.y * time > DOWNWALL)
	{
		this->circle.setPosition(this->circle.getPosition().x, DOWNWALL - 2 * this->circle.getRadius() - 2);
		this->V.y = -abs(V.y) * 0.8;
		moveY = 0;
	}
}

/***********************************************************************************/
double	enemy::calculateDistance(sf::Vector2f &position, sf::Vector2f &otherPosition)
{
	double val = sqrt(pow(position.x - otherPosition.x, 2) + pow(position.y - otherPosition.y, 2));
	if (val != 0)
		return val;
	else
		return 0.0000001;
}

/***********************************************************************************/
double	enemy::apply1Rule(double averageV, double verticle, double pith)
{
	return pith*(averageV - verticle);
}

/***********************************************************************************/
double	enemy::apply2Rule(double posX, double posX2, double distance, double pith)
{
	return pith * (posX2 - posX) * (distance - *this->averageDistance) / distance;
}

/***********************************************************************************/
double	enemy::apply3Rule(double PosX1, double PosX2, double distance, double pith, double minDistance)
{
	return pith * ((PosX2 - PosX1) * minDistance / distance - (PosX2 - PosX1));
}

/***********************************************************************************/
void enemy::manageColliding(int index, double distance)
{
	double minDistance = this->circle.getRadius() + parametersOfOtherMates[index].radius  + minDistanceConst;
	if (distance < minDistance)
	{
		this->moveX -= apply3Rule(this->getCenter().x, parametersOfOtherMates[index].position.x, distance, collidingEnemiesPith, minDistance);
		this->moveY -= apply3Rule(this->getCenter().y, parametersOfOtherMates[index].position.y, distance, collidingEnemiesPith, minDistance);
	}
}

/***********************************************************************************/
sf::Vector2f	enemy::applyAllRules(sf::Vector2f &AverageV, double &averageDistance)
{
	sf::Vector2f vect;
	vect.x = 0;
	vect.y = 0;

	// regu³a pierwsza 
	vect.x += enemy::apply1Rule(average.x, vect.x, this->parameters.pith);
	vect.y += enemy::apply1Rule(average.y, vect.y, this->parameters.pith);

	for (int i = 0; i < this->parametersOfOtherMates.size(); ++i)
	{
		// regu³a druga
		double distance = calculateDistance(this->getCenter(), parametersOfOtherMates[i].position);
		// regu³a trzecia

		vect.x += apply2Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->parameters.pith);
		vect.y += apply2Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->parameters.pith);

		manageColliding(i, distance);
	}

	return vect;
}

/***********************************************************************************/
void			enemy::randomAccelerationSign(float &accelerationVal) 
{
	int val = rand() % 3;
	if (val == 0)
		accelerationVal = abs(this->parameters.acceleration.x);
	else if (val == 1)
		accelerationVal = -abs(this->parameters.acceleration.x);
	else
		accelerationVal = 0;
}

/***********************************************************************************/