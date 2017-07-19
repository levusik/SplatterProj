#pragma once

#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include <fstream>
#include <cstdlib>
#include "player.hpp"
#include <iostream>
#include "HPBar.h"


/*

FLIGHTY,		// przeciwnik zazwyczaj ucieka, nastawiony defensywnie (znacze boosty w obronie)
// umiej�tno�� specjalna :   zwi�ksza swoj� obron� i zatrzymuje gracza
// du�e- bardzo du�e grupki

AGRESSIVE,		// przeciwnik zazwyczaj walczy, dobry Atak (nastawiony na DPS) , gorsza obrona
// umiej�tno�� specjalna : zwi�szenie cz�stotlwio�ci i dotkliwo�ci obra�e� ( animacja :
// powi�ksza si� by z czasem powr�ci� do starego rozmiaru).
// �rednie-du�e grupki

HEAVY,			// przeciwnik kontynuuje swoj� podr�, dopiero zbli�ony na odpowiedn� odleg�o�� zacznie atakowa�
// szybko traci zaintersowanie, bardzo du�a obrona, dotkliwe obra�enia, aczolwiek powolny
// umiej�tno�� specjalna : berserk - zmiejsza otrzymywane obra�enia, zwi�ksza pr�dko��, zadaje bardziej dotkliwej
// obra�enia
// ma�e- �rednie grupki

RUNNERS,		// przeciwnicy walcz� tylko w du�ej grupie w przeciwnym wypadku uciekaj�
// pojedy�czo nie stanowi� �adnego zagro�enia, aczkolwiek w grupie s� wyj�tkowo gro�ni
// ma�y damage, �rednia obrona, ma�o zdrowia,
// umiej�tno�� specjalna : atak grupowy - ca�a grupa rzuca si� na przeciwnika
// du�e-olbrzymie grupki

BEAST,			// olbrzymi, skrajnie agresywni przeciwnicy, bardzo du�y damage, ma�a obrona, du�o zdrowia
// podr�uj� samotnie, gdy raz zostanie zwi�zany walk� b�dzie tropi� ju� do jego i gracza �mierci
// umiej�tno�� specjalna : obszar�wka , szar�a
// pojedy�cze osobniki

PREDATOR,		// najbardziej skomplikowany schemat AI, pr�buj� okr��y� przeciwnika, zaprowadzi� go w k�t ;
// du�a szybko�� i wszystko inne raczej �rednie
// umiej�tno�� specjalna : taktyka, atak grup�
// �rednie grupki

BITTERS,		// nastawi� si� na szybkie i b�yskawiczne ataki, po czym na "taktyczny odwr�t", przegrupowanie i wracamy do atak�w
// du�y damage i szybko��, wszystko inne tragiczne.
// umiej�tno�� specjalna : atak grup�, zatrucie Gracza

ELEMENTS,		// nastawi�j� si� na d�u�sz� met�, du�a obrona i zdrowie , kiepski damage ale obra�enia s� zadawane po czasie, �rednia pr�dko��
// umiej�tno�� specjalna : ataki zadaj� obra�enia po jakim� czasie ( np. od ognia, zatru� )

*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// g��wna klasa 
class enemy : public sf::Sprite, public aliveCreature
{
public:

	// Konstruktor
	enemy();

	////////////////////////////
	//settery 
	void				setID(int id);
	void				alertEnemy();
	void				intrigueEnemy();
	void				setDamageParameters(damageParameters newParams);
	////////////////////////////


	///////////////////////////////////////////////////////////////
	// gettery
	int					getGroupIndex();
	int					getWorth();
	int					getID();
	bool				isIDLE();
	bool				hasCreatureDetectedPlayer();
	bool				isCreatureAlive();
	bool				isEnemyIntrigued();
	bool				canBeHittedByExplosion();
	double				getSizeOfView();
	double				getRandomVal(int min, int max);
	double				getHP();
	sf::Vector2f		getCenter();
	sf::Vector2f		getV();
	sf::CircleShape		getCircle();
	sf::VertexArray		getVertex();
	HPbar				getHPBar() const;
	///////////////////////////////////////////////////////////////


	/***********************************************************************************/
	//	metody kt�re w zale�no�ci od od tego z jakim przeciwnkiem mamy 
	//	do czynienia b�dziemy override'owa� 

	virtual void	create(int groupIndex, splatTemplate enemyTemplate);
	virtual void	update(sf::Vector2f *averageV, double *averageDistance, player *Player);
	virtual	void	dealDamage(damageParameters Damage, int IDOfHurtedEnemy);
	virtual void	setParametersOfOtherMates(std::vector<otherMatesParameters> &ParametersOfOtherMates);

protected:
	
	virtual void manageAIScheme();

	virtual void IDLEState() {}
	virtual void IDLESChangeToDiffState() {}

	virtual void intriguedState() {}
	virtual void intriguedSChangeToDiffState() {}

	virtual void makingDecisionState() {}
	virtual void makingDecisionChangeToDiffState() {}

	virtual void fleeState() {}
	virtual void fleeChangeToDiffState() {}

	virtual void agressiveState() {}
	virtual void agressiveChangeToDiffState() {}

	virtual void chargeState() {}
	virtual void chargeChangeToDiffState() {}

	virtual void attackState() {}
	virtual void attackChangeToDiffState() {}

	virtual void specialAbility() {}
	virtual void specialAbilityChangeToDiffState() {}

	virtual bool isWorthAttacking() { return true; }

	virtual void reactToGettingHit() {}
	virtual void gettingHitChangeStates() {}

	virtual void playAttackAnim() {}
	virtual void randomizeMovement();

	virtual void manageMovement();
	virtual void moveTowardsPlayer();
	virtual void runAwayFromPlayer();
	virtual void normalMovement();

	// specjalna metoda kt�r� b�dziemy wywo�ywa� gdy zmienimy stan na agresywny
	//	(r�ni przeciwnicy b�d� reagowa� na rozpoczecie walki)
	virtual void changeToAgressiveState() {
		this->currentState = stateOfEnemy::AGRESSIVE;
		this->circle.setFillColor(this->agressiveStateColor);
	};

	/***********************************************************************************/


	/***********************************************************************************/
	//	 metody potrzebne do algorytmu boid�w oraz do update'u

	virtual void	limitVector();
	void			manageVertexLine();
	void			calculateTrygFunc();
	void			handleBoundingRect();
	double			calculateDistance(sf::Vector2f &position, sf::Vector2f &otherPosition);
	double			apply1Rule(double averageV, double verticle, double pith);
	double			apply2Rule(double posX, double posX2, double distance, double pith);
	double			apply3Rule(double PosX1, double PosX2, double distance, double pith ,double minDistance);
	void			manageColliding(int index, double distance);
	sf::Vector2f	applyAllRules(sf::Vector2f &AverageV, double &averageDistance);
	void			randomAccelerationSign(float &accelerationVal);
	/***********************************************************************************/

	// parametry kt�re wczytamy sobie elegancko z "bazy danych"
	
	double HP;
	double damage;
	double armor;
	double agility;
	double time;
	double percOfMaxHPWhenEnemyFlee;

	// parametry wroga
	splatTemplate parameters;
	double	maxHP;
	bool isInFightingState;
	// parametry obra�e� kt�re zadaje agent
	damageParameters dmgParameters;
	int		idOfHurtedEnemy;
	
	bool	movememtLock = 0;
	bool	chargeLock = 0;
	bool	fleeLock = 0;


	double moveX = 0, moveY = 0;
	int accelerationXSign = 1;
	int accelerationYSign = 1;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// parametry czysto techniczne 

	bool moveToPlayer = 0;
	bool runAway = 0;
	bool groupAnimal = 0;

	// kolorki
	sf::Color colorOfHPBar;
	sf::Color IdleStateColor; ;
	sf::Color IntriguedStateColor;
	sf::Color makingDecisionStateColor;
	sf::Color agressiveStateColor;
	sf::Color chargeStateColor;
	sf::Color attackStateColor;
	sf::Color fleeStateColor;
	sf::Color specialAbilityStateColor;
	sf::Color circleColor;


	// stan FSM 
	stateOfEnemy currentState;

	// rysunki !
	sf::CircleShape		circle;
	class HPbar			HPRectangle;
	sf::VertexArray		speedVector;


	// zmienne kt�re u�atwi� nam identyfikacj� 
	int		ID;
	int		groupIndex;


	bool playerHurtedGroupMate, isRangeUnit, hasCharged;
	bool isGroupIntrigued;
	bool isGroupAgressive;

	// pr�dko�� liniowa x i y
	sf::Vector2f V;

	// przyspieszenie 
	sf::Vector2f acceleration;

	// �rednie pr�dko�ci ca�ej grupy 
	sf::Vector2f average;


	// zapisujemy sobie adresy parametr�w kt�re wrzucamy do update'a 
	sf::Vector2f *averageV;
	double		 *averageDistance;
	player		 *playerPointer;

	// je�eli doszed� do ko�ca planszy chcemy przyspieszy� proces losowania ruchu
	double timeAdd;


	// zmienne potrzebne do ruchu w kierunku gracza
	double sinA, cosA;
	double distanceBtwCreatureAndPlayer;
	double slowDistance;


	// boosty i deboosty 
	double timeOfDeceleration;
	double speedBuff, accelerationBuff, attackBuff, defenseBuff;





	sf::Clock		makingDecisionTime;
	sf::Clock		delayBtwRandomizeMovement;
	sf::Clock		delayBtwChangingMove;
	sf::Clock		delayBtwAttacking;
	std::vector<otherMatesParameters> parametersOfOtherMates;



	// zegar potrzebny do odmierzenia czasu reakcji
	sf::Clock timeToMakeDecision;

	// czas po kt�rym mo�e zosta� znowu uderzony przez obszar�wk�
	sf::Clock terrainDamageClock;

	// czas potrzebny do ruchu
	sf::Clock clock;
	double prevTime;
	////////////////////////////////////////////////////////////////////
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "flightyEnemy.hpp"
#include "agressiveEnemy.hpp"
#include "heavyEnemy.hpp"
#include "runnersEnemy.hpp"
#include "beastEnemy.hpp"
#include "bittersEnemy.hpp"
#include "predatorEnemy.hpp"
#include "elementsEnemy.hpp"