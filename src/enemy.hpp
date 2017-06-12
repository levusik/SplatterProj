#pragma once
#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include <fstream>
#include <cstdlib>


/*
Zamys� :

ka�dy przeciwnik b�dzie obiektem klasy dziedicznej po klasie enemy.
podczas rozpocz�cia fali czy innego zdarzenia kt�re wygeneruje przeciwnik�w b�dziemy losowa� sobie kilka pozycji
(ilo�� zale�na od rodzaju przeciwnik�w oraz poziomu trudno�ci ) kt�re �adnie nazwa�em spawn pointami 
te pozycje b�d� pe�ni� funkcje pozycji startowych nowo-powsta�ych przeciwnik�w.

Ka�da grupa b�dzie sk�ada�a si� z kilku-kilkunastu cz�onk�w. Chcemy �eby ka�da grupa porusza�a si�
w mniej wi�cej tym samym kierunku a zgrupowanie ich pozwoli nam na �atwe obliczenie tego, poniewa� b�dziemy dzi�ki temu �atwo pobra�
pr�dko�� X i Y ka�dego poszczeg�lnego orka w grupie oraz obliczy� �redni� warto�� z tego, po czym przy update'cie b�dziemy dodawa� t� �redni� do
pr�dko�� orka (  http://www.algorytm.org/sztuczna-inteligencja/boidy.html  )

ka�dy przeciwnik b�dzie zachowywa� si� w ten spos�b

				prototyp Schematu grafowowego ( patrz. zeszyt od infy)
					
					Utworzenie Przeciwnika
				 /							\
				/							 \
je�eli nie wykryto gracza	                  \  je�eli wykryto gracza
 	       /  														\
		  /															 \
		 /															  \
----> Szw�daj si�					  |---------------------->  Id� w kierunku gracza <---
|		 |							  |										|			  |
|________|							  |										|_____________|
dop�ki nie wykryto gracza			  |									Je�eli przekroczono podany dystans
w przeciwnym wypadku ------------------									 to prze��cz w stan szar�y
																			|
																		-------------
																		|			|										 |-----------|
																		|   Szar�a  |  ----------------------------------->  |	 ATAK	 |	 W tym momencie wkracza silnik gry i obs�uga taku
																		|			|	   je�eli wystarczaj�co blisko		 |-----------|
																		-------------


*/


# define GETSEC(x) x.getElapsedTime().asSeconds()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// g��wna klasa abstrakcyjna 
class enemy : public sf::Sprite
{
public:
	enemy()
		: speedVector(sf::Lines, 6)
	{
		this->isAlive = true;

	};
	sf::CircleShape getCircle()
	{
		return this->circle;
	}

	// gettery
	int					getGroupIndex()
	{
		return this->groupIndex;
	}
	Verticle			getV()
	{
		return this->V;
	}
	double				getSizeOfView()
	{
		return this->sizeOfView;
	}
	double				getAngleOfView()
	{
		return this->angleOfView;
	}
	Verticle			getCenter()
	{
		// mo�na to zmie�ci� w jednej linijce ale syntatic sugar zawsze na propsie
		Verticle position;
		position.x = this->circle.getPosition().x + this->circle.getRadius();
		position.y = this->circle.getPosition().y + this->circle.getRadius();
		return position;
	}
	bool				hasCreatureDetectedPlayer()
	{
		return this->playerDetected;
	}
	virtual	void		dealDamage(bulletParameters collidedBullet) = 0;
	sf::VertexArray		getVertex()
	{
		return this->speedVector;
	}
	bool				isCreatureAlive()
	{
		return this->isAlive;
	}
	void				alertEnemy()
	{
		this->playerDetected = true;
	}
	double getRandomVal(int min, int max)
	{
		return rand() % (max - min) + min;
	}
	int	   getWorth()
	{
		return this->worth;
	}

protected:
	///////////////////////////////////////////////////////////////////////////////////////////////
	// schemat AI przeciwnika rozpisany w zeszycie 

	virtual void walkingArround(Verticle playerPos) = 0;
	virtual void reactToPlayer(Verticle playerPos) = 0;
	virtual void movingTowardsPlayer(Verticle playerPos) = 0;
	virtual void charge() = 0;
	virtual void attackPlayer() = 0;
	double calculateDistance(Verticle &position, Verticle &otherPosition)
	{
		double val = sqrt(pow(position.x - otherPosition.x, 2) + pow(position.y - otherPosition.y, 2));
		if (val != 0)
			return val;
		else
			return 0.0001;
	}
	double apply1Rule(double averageV, double verticle, double pith)
	{
		return pith*(averageV - verticle);
	}
	double apply2Rule(double posX, double posX2, double distance, double pith)
	{
		return pith * (posX2 - posX) * (distance - averageDistance) / distance;
	}
	double apply3Rule(double PosX1, double PosX2, double distance, double pith)
	{
		return pith * ((PosX1 - PosX2) * this->minDistanceFromMates / distance - (PosX1 - PosX2));
	}
	virtual void   manageAcceleration() = 0;


	void manageVertexLine(double x, double y, double sinA, double cosA)
	{
		this->speedVector[0].position.x = this->getCenter().x;
		this->speedVector[0].position.y = this->getCenter().y;

		this->speedVector[1].position.x = this->speedVector[0].position.x + x * 20 * sinA;	// mno�ymy przez 3 �eby bardziej by�o to widoczne
		this->speedVector[1].position.y = this->speedVector[0].position.y;	// mno�ymy przez 3 �eby bardziej by�o to widoczne

		this->speedVector[2] = this->speedVector[0];

		this->speedVector[3].position.x = this->speedVector[0].position.x;
		this->speedVector[3].position.y = this->speedVector[0].position.y + y * 20 * cosA;

		this->speedVector[4] = this->speedVector[0];

		this->speedVector[5].position.x = speedVector[1].position.x;
		this->speedVector[5].position.y = speedVector[3].position.y;

	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	// cechy kt�re ka�dy przeciwnik b�dzie mia�

	// do kt�rej grupy nale�y przeciwnik ( potrzebne potem )
	int groupIndex;
	bool playerDetected, isRangeUnit, hasCharged;

	// pr�dko�� liniowa x i y
	Verticle V;

	// �rednie pr�dko�ci ca�ej grupy 
	Verticle average;

	// wektor pr�dko�ci
	sf::VertexArray speedVector;

	// �rednia odleg�o�� od innych cz�onk�w grupy
	double averageDistance;

	// zasi�g i k�t widoku przeciwnika
	double sizeOfView, angleOfView, pith, minDistanceFromMates;

	// zmienne potrzebne do ruchu w kierunku gracza
	double movementToPlayerX, movementToPlayerY, minSpeed, maxSpeed;
	double acceleration;
	double sinA, cosA;
	double chargeDistance;
	double safeDistance;
	double distanceBtwCreatureAndPlayer;
	double slowDistance;
	double timeOfDeceleration;

	// zmienne potrzebne do kolizji z pociskiem
	// parametry wroga
	bool	isAlive;
	double	armor, HP, Damage;
	int		rangeOfWeapon;


	sf::CircleShape circle;
	sf::Clock		delayBtwChangingMove;
	std::vector<otherMatesParameters> parametersOfOtherMates;

	stateOfEnemy currentState;

	// warto�� splata ( warto�� kt�r� dodajemy do kasy gracza)
	int		worth;

	// kolorki !
	sf::Color agressiveModeColor;
	sf::Color reactingtoPlayerColor;
	sf::Color ChargingColor;
	sf::Color fleeingColor;
	sf::Color attackingPlayerColor;
	sf::Color passiveModeColor;


	// czas po kt�rym jednostka podejmuje decyzj� jak� akcj� wykona� 
	double timeToMakeDecisionInSec;
	sf::Clock timeToMakeDecision;

	////////////////////////////////////////////////////////////////////
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// g��wna klasa ork�w 
class splat : public enemy
{
public:
	// konstruktor kt�ry utworzy "blanka"
	splat()
	{
		this->armor = 0;
		this->Damage = 0;
		this->groupIndex = -1;
		this->HP = 0;
		this->isRangeUnit = false;
		this->playerDetected = false;
		this->rangeOfWeapon = 0;
		this->V.x = 0;
		this->V.y = 0;
		this->pith = 0.025;
		this->worth = 1;


	}
	void create(int groupIndex, splatTemplate orkUnit);
	void update(Verticle &averageV, double &averageDistance, Verticle playerPos);
	void setParametersOfOtherMates(std::vector<otherMatesParameters> &ParametersOfOtherMates);
	virtual void dealDamage(bulletParameters collidedBullet) override;

private:
	// co ile nast�puje zmiana kierunku ruchu podczas "szw�dania si� przeciwnika" 
	float changeMoveBy;


	virtual void walkingArround(Verticle playerPos) override;
	virtual void reactToPlayer(Verticle playerPos)  override;
	virtual void movingTowardsPlayer(Verticle playerPos) override;
	virtual void charge() override;
	virtual void attackPlayer() override;
	virtual void manageAcceleration() override;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////