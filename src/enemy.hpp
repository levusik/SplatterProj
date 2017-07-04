#pragma once

#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include <fstream>
#include <cstdlib>
#include "player.hpp"
#include <iostream>
#include "HPBar.h"




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// g³ówna klasa 
class enemy : public sf::Sprite, public aliveCreature
{
public:
	
	// Konstruktor
	enemy()
		: speedVector(sf::Lines, 6)
	{
		this->isAlive = true;
		timeAdd = 0;
		VxMustBePlus =	false;
		VxMustBeNeg  =	false;
		VyMusBePlus  =	false;
		VyMusBeNeg   =	false;

	};
	
	////////////////////////////
	//settery 
	void setID(int id)
	{
		this->ID = id;
	}
	void				alertEnemy()
	{
		this->playerHurtedGroupMate = true;
	}
	void				intrigueEnemy()
	{
		this->isGroupIntrigued = true;
	}
	void				setDamageParameters(damageParameters newParams)
	{
		this->dmgParameters = newParams;
	}
	////////////////////////////


	///////////////////////////////////////////////////////////////
	// gettery
	int					getGroupIndex()
	{
		return this->groupIndex;
	}
	int					getWorth()
	{
		return this->worth;
	}
	int					getID()
	{
		return this->ID;
	}
	bool				isIDLE()
	{
		return this->currentState == stateOfEnemy::IDLE;
	}
	bool				hasCreatureDetectedPlayer()
	{
		return this->playerHurtedGroupMate;
	}
	bool				isCreatureAlive()
	{
		return this->isAlive;
	}
	bool				isEnemyIntrigued()
	{
		return this->isGroupIntrigued;
	}
	bool				canBeHittedByExplosion()
	{
		return this->terrainDamageClock.getElapsedTime().asSeconds() > terrainDmgLock;
	}
	double				getSizeOfView()
	{
		return this->sizeOfView;
	}
	double				getAngleOfView()
	{
		return this->angleOfView;
	}
	double				getRandomVal(int min, int max)
	{
		return rand() % (max - min) + min;
	}
	double				getHP()
	{
		return this->HP;
	}
	sf::Vector2f		getCenter()
	{
		// mo¿na to zmieœciæ w jednej linijce ale syntatic sugar zawsze na propsie
		sf::Vector2f position;
		position.x = this->circle.getPosition().x + this->circle.getRadius();
		position.y = this->circle.getPosition().y + this->circle.getRadius();
		return position;
	}
	sf::Vector2f		getV()
	{
		return this->V;
	}
	sf::CircleShape		getCircle()
	{
		return this->circle;
	}
	sf::VertexArray		getVertex()
	{
		return this->speedVector;
	}
	HPbar				getHPBar() const
	{
		return this->HPRectangle;
	}
	///////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////
	// schemat AI przeciwnika rozpisany w zeszycie 


	/***********************************************************************************/
	//	metody które w zale¿noœci od od tego z jakim przeciwnkiem mamy 
	//	do czynienia bêdziemy override'owaæ 

	virtual void	create(int groupIndex, splatTemplate enemyTemplate)
	{

	}
	virtual void	update(sf::Vector2f *averageV, double *averageDistance, player *Player)
	{
		this->manageAIScheme(averageV, averageDistance, Player);
		
		this->HPRectangle.update(this->getCenter(), this->HP, true , this->circle.getRadius());
	}
	virtual void	setParametersOfOtherMates(std::vector<otherMatesParameters> &ParametersOfOtherMates)
	{

	}
	virtual	void	dealDamage(damageParameters Damage)
	{}


	protected:
		
		virtual void manageAIScheme(sf::Vector2f *averageV, double *averageDistance, player *Player)		{}

		virtual void IDLEState()						{}
		virtual void IDLESChangeToDiffState()			{}
		
		virtual void intriguedState()					{}
		virtual void intriguedSChangeToDiffState()		{}
		
		virtual void makingDecisionState()				{}
		virtual void makingDecisionChangeToDiffState()	{}

		virtual void fleeState()						{}
		virtual void fleeChangeToDiffState()			{}

		virtual void agressiveState()					{}
		virtual void agressiveChangeToDiffState()		{}

		virtual void chargeState()						{}
		virtual void chargeChangeToDiffState()			{}

		virtual void attackState()						{}
		virtual void attackChangeToDiffState()			{}

		virtual void specialAbility()					{}
		virtual void specialAbilityChangeToDiffState()	{}

		virtual bool isWorthAttacking()		{return true;}
		
		virtual void reactToGettingHit()		{}
		virtual void gettingHitChangeStates()	{}

		virtual void playAttackAnim() {}
		virtual void randomizeMovement() {}

	/***********************************************************************************/


	/***********************************************************************************/
	//	 metody potrzebne do algorytmu boidów oraz do update'u
		
	void			manageVertexLine(double x, double y, double sinA, double cosA)
		{
			this->speedVector[0].position.x = this->getCenter().x;
			this->speedVector[0].position.y = this->getCenter().y;

			this->speedVector[1].position.x = this->speedVector[0].position.x + x * 20 * sinA;	// mno¿ymy przez 3 ¿eby bardziej by³o to widoczne
			this->speedVector[1].position.y = this->speedVector[0].position.y;	// mno¿ymy przez 3 ¿eby bardziej by³o to widoczne

			this->speedVector[2] = this->speedVector[0];

			this->speedVector[3].position.x = this->speedVector[0].position.x;
			this->speedVector[3].position.y = this->speedVector[0].position.y + y * 20 * cosA;

			this->speedVector[4] = this->speedVector[0];

			this->speedVector[5].position.x = speedVector[1].position.x;
			this->speedVector[5].position.y = speedVector[3].position.y;

		}
	void			limitVector()
		{
			if (std::abs(this->V.x) > this->maxSpeed)
				this->V.x *= 0.9;
			if (std::abs(this->V.y) > this->maxSpeed)
				this->V.y *= 0.9;

		}
	void			calculateTrygFunc()
		{
			this->sinA = (this->playerPointer->getCenter().y - this->getCenter().y) / this->distanceBtwCreatureAndPlayer;
			this->cosA = (this->playerPointer->getCenter().x - this->getCenter().x) / this->distanceBtwCreatureAndPlayer;

		}
	void			handleBoundingRect()
	{

		if (this->circle.getPosition().x < LEFTWALL + std::abs(V.x))
		{
			this->circle.setPosition(LEFTWALL + 1, this->circle.getPosition().y);
			this->V.x *= -0.5;
			timeAdd = 2.f;
			VxMustBePlus = true;
		}
		else if (this->circle.getPosition().x + 2*this->circle.getRadius() > RIGHTWALL - std::abs(V.x))
		{
			this->circle.setPosition(RIGHTWALL - 2*this->circle.getRadius() - 1, this->circle.getPosition().y);
			this->V.x *= -0.5;
			timeAdd = 2.f;
			VxMustBeNeg = true;
		}
		
		if (this->circle.getPosition().y  < UPWALL + std::abs(V.y))
		{
			this->circle.setPosition(this->circle.getPosition().x, UPWALL + 1);
			this->V.y *= -0.5;
			timeAdd = 2.f;
			VyMusBePlus = true;
		}

		else if (this->circle.getPosition().y + 2*this->circle.getRadius() > DOWNWALL - std::abs(V.y))
		{
			this->circle.setPosition(this->circle.getPosition().x, DOWNWALL - 2*this->circle.getRadius() - 1);
			this->V.y *= -0.5;
			timeAdd = 2.f;
			VyMusBeNeg = true;
		}
	}

	double			calculateDistance(sf::Vector2f &position, sf::Vector2f &otherPosition)
	{
		double val = sqrt(pow(position.x - otherPosition.x, 2) + pow(position.y - otherPosition.y, 2));
		if (val != 0)
			return val;
		else
			return 0.0001;
	}
	double			apply1Rule(double averageV, double verticle, double pith)
	{
		return pith*(averageV - verticle);
	}
	double			apply2Rule(double posX, double posX2, double distance, double pith)
	{
		return pith * (posX2 - posX) * (distance - *this->averageDistance) / distance;
	}
	double			apply3Rule(double PosX1, double PosX2, double distance, double pith)
	{
		return pith * ((PosX1 - PosX2) * this->minDistanceFromMates / distance - (PosX1 - PosX2));
	}
	sf::Vector2f	applyAllRules(sf::Vector2f &AverageV, double &averageDistance)
	{
		sf::Vector2f vect;
		vect.x = 0;
		vect.y = 0;

		// regu³a pierwsza 
		vect.x += enemy::apply1Rule(average.x, vect.x, this->pith);
		vect.y += enemy::apply1Rule(average.y, vect.y, this->pith);

		for (int i = 0; i < this->parametersOfOtherMates.size(); ++i)
		{
				// regu³a druga
				double distance = calculateDistance(this->getCenter(), parametersOfOtherMates[i].position);
				
				vect.x += apply2Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->pith);
				vect.y += apply2Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->pith);

				// regu³a trzecia
				if (distance < this->minDistanceFromMates)
				{
					vect.x += apply3Rule(this->getCenter().x, parametersOfOtherMates[i].position.x, distance, this->pith);
					vect.y += apply3Rule(this->getCenter().y, parametersOfOtherMates[i].position.y, distance, this->pith);
				}
		}

		return vect;
	}
	/***********************************************************************************/

	///////////////////////////////////////////////////////////////////////////////////////////////
	// cechy które ka¿dy przeciwnik bêdzie mia³

	// zmienne które u³atwi¹ nam identyfikacjê 
	int groupIndex;
	int		ID;

	
	bool playerHurtedGroupMate, isRangeUnit, hasCharged;
	bool isGroupIntrigued;
	bool isGroupAgressive;
	
	// prêdkoœæ liniowa x i y
	sf::Vector2f V;

	// œrednie prêdkoœci ca³ej grupy 
	sf::Vector2f average;

	// wektor prêdkoœci
	sf::VertexArray speedVector;

	// zasiêg i k¹t widoku przeciwnika
	double sizeOfView, angleOfView, pith, minDistanceFromMates;
	double minDistanceFromPlayer;

	// zapisujemy sobie adresy parametrów które wrzucamy do update'a 
	sf::Vector2f *averageV;
	double		 *averageDistance;
	player		 *playerPointer;

	// je¿eli doszed³ do koñca planszy chcemy przyspieszyæ proces losowania ruchu
	double timeAdd;

	// idiotyczne i bezsensowne rozwi¹zanie problemu 
	//     ||
	//     \/
	bool VxMustBePlus, VxMustBeNeg, VyMusBePlus, VyMusBeNeg;



	// zmienne potrzebne do ruchu w kierunku gracza
	double minSpeed, maxSpeed;
	double acceleration;
	double sinA, cosA;
	double chargeDistance;
	double safeDistance;
	double distanceBtwCreatureAndPlayer;
	double slowDistance;
	double timeOfDeceleration;
	double speedBuff, attackBuff;

	// zmienne potrzebne do kolizji z pociskiem
	// parametry wroga
	bool	isAlive;
	double	armor, Damage;
	double	maxHP;
	int		rangeOfWeapon;



	sf::CircleShape		circle;
	class HPbar			HPRectangle;


	sf::Clock		delayBtwChangingMove;
	std::vector<otherMatesParameters> parametersOfOtherMates;

	stateOfEnemy currentState;

	// wartoœæ splata ( wartoœæ któr¹ dodajemy do kasy gracza)
	int		worth;

	// kolorki !
	sf::Color colorOfHPBar;
	sf::Color IdleStateColor;
	sf::Color IntriguedStateColor;
	sf::Color makingDecisionStateColor;
	sf::Color agressiveStateColor;
	sf::Color chargeStateColor;
	sf::Color attackStateColor;
	sf::Color fleeStateColor;
	sf::Color specialAbilityStateColor;


	damageParameters dmgParameters;


	// czas po którym jednostka podejmuje decyzjê jak¹ akcjê wykonaæ 
	double timeToMakeDecisionInSec;
	sf::Clock timeToMakeDecision;

	// czas po którym mo¿e zostaæ znowu uderzony przez obszarówkê
	sf::Clock terrainDamageClock;

	////////////////////////////////////////////////////////////////////
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// podstawowa klasa przeciwnika w której zapiszemy 
// wszystkie domyœlne zachowania AI.
class splat : public enemy
{
public:
	// konstruktor który utworzy "blanka"
	splat()
	{
		this->armor = 0;
		this->Damage = 0;
		this->groupIndex = -1;
		this->HP = 0;
		this->isRangeUnit = false;
		this->playerHurtedGroupMate = false;
		this->rangeOfWeapon = 0;
		this->V.x = 0;
		this->V.y = 0;
		this->pith = 0.02;
		this->worth = 1;
	}
	// u¿ywamy domyœlnego update'a


	virtual void	setParametersOfOtherMates(std::vector<otherMatesParameters> &ParametersOfOtherMates) override;
	virtual void	dealDamage(damageParameters damage) override;
	virtual void	create(int groupIndex, splatTemplate enemyTemplate) override;


protected:

	/******************************************************************************************************/
	/*		Nadpisanie metod enemy																		  */
	virtual void manageAIScheme(sf::Vector2f *averageV, double *averageDistance, player *Player) override;

	virtual void IDLEState()								override;
	virtual void IDLESChangeToDiffState()					override;

	virtual void intriguedState()							override;
	virtual void intriguedSChangeToDiffState()				override;

	virtual void makingDecisionState()						override;
	virtual void makingDecisionChangeToDiffState()			override;

	virtual void fleeState()								override;
	virtual void fleeChangeToDiffState()					override;


	virtual void agressiveState()							override;
	virtual void agressiveChangeToDiffState()				override;

	virtual void chargeState()								override;
	virtual void chargeChangeToDiffState()					override;

	virtual void attackState()								override;
	virtual void attackChangeToDiffState()					override;

	virtual void specialAbility()							override;
	virtual void specialAbilityChangeToDiffState()			override;

	virtual bool isWorthAttacking()							override;

	virtual void reactToGettingHit()						override;
	virtual void gettingHitChangeStates()					override;

	virtual void randomizeMovement()						override;

	

	/******************************************************************************************************/

	// co ile nastêpuje zmiana kierunku ruchu podczas "szwêdania siê przeciwnika" 
	float changeMoveBy;
	double rangeOfWeapon;
	double delayBtwAttacksInSecs;
	bool   isInFightingState;

	sf::Clock delayBtwRandomizeMovement;
	sf::Clock delayBtwBoidAlgorithm;
	sf::Clock makingDecisionDelay;
	sf::Clock movementDelay;
	sf::Clock delayBtwAttacksClock;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////