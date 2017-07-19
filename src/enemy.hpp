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
// umiejêtnoœæ specjalna :   zwiêksza swoj¹ obronê i zatrzymuje gracza
// du¿e- bardzo du¿e grupki

AGRESSIVE,		// przeciwnik zazwyczaj walczy, dobry Atak (nastawiony na DPS) , gorsza obrona
// umiejêtnoœæ specjalna : zwiêszenie czêstotlwioœci i dotkliwoœci obra¿eñ ( animacja :
// powiêksza siê by z czasem powróciæ do starego rozmiaru).
// œrednie-du¿e grupki

HEAVY,			// przeciwnik kontynuuje swoj¹ podró¿, dopiero zbli¿ony na odpowiedn¹ odleg³oœæ zacznie atakowaæ
// szybko traci zaintersowanie, bardzo du¿a obrona, dotkliwe obra¿enia, aczolwiek powolny
// umiejêtnoœæ specjalna : berserk - zmiejsza otrzymywane obra¿enia, zwiêksza prêdkoœæ, zadaje bardziej dotkliwej
// obra¿enia
// ma³e- œrednie grupki

RUNNERS,		// przeciwnicy walcz¹ tylko w du¿ej grupie w przeciwnym wypadku uciekaj¹
// pojedyñczo nie stanowi¹ ¿adnego zagro¿enia, aczkolwiek w grupie s¹ wyj¹tkowo groŸni
// ma³y damage, œrednia obrona, ma³o zdrowia,
// umiejêtnoœæ specjalna : atak grupowy - ca³a grupa rzuca siê na przeciwnika
// du¿e-olbrzymie grupki

BEAST,			// olbrzymi, skrajnie agresywni przeciwnicy, bardzo du¿y damage, ma³a obrona, du¿o zdrowia
// podró¿uj¹ samotnie, gdy raz zostanie zwi¹zany walk¹ bêdzie tropiæ ju¿ do jego i gracza œmierci
// umiejêtnoœæ specjalna : obszarówka , szar¿a
// pojedyñcze osobniki

PREDATOR,		// najbardziej skomplikowany schemat AI, próbuj¹ okr¹¿yæ przeciwnika, zaprowadziæ go w k¹t ;
// du¿a szybkoœæ i wszystko inne raczej œrednie
// umiejêtnoœæ specjalna : taktyka, atak grup¹
// œrednie grupki

BITTERS,		// nastawi¹ siê na szybkie i b³yskawiczne ataki, po czym na "taktyczny odwrót", przegrupowanie i wracamy do ataków
// du¿y damage i szybkoœæ, wszystko inne tragiczne.
// umiejêtnoœæ specjalna : atak grup¹, zatrucie Gracza

ELEMENTS,		// nastawi¹j¹ siê na d³u¿sz¹ metê, du¿a obrona i zdrowie , kiepski damage ale obra¿enia s¹ zadawane po czasie, œrednia prêdkoœæ
// umiejêtnoœæ specjalna : ataki zadaj¹ obra¿enia po jakimœ czasie ( np. od ognia, zatruæ )

*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// g³ówna klasa 
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
	//	metody które w zale¿noœci od od tego z jakim przeciwnkiem mamy 
	//	do czynienia bêdziemy override'owaæ 

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

	// specjalna metoda któr¹ bêdziemy wywo³ywaæ gdy zmienimy stan na agresywny
	//	(ró¿ni przeciwnicy bêd¹ reagowaæ na rozpoczecie walki)
	virtual void changeToAgressiveState() {
		this->currentState = stateOfEnemy::AGRESSIVE;
		this->circle.setFillColor(this->agressiveStateColor);
	};

	/***********************************************************************************/


	/***********************************************************************************/
	//	 metody potrzebne do algorytmu boidów oraz do update'u

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

	// parametry które wczytamy sobie elegancko z "bazy danych"
	
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
	// parametry obra¿eñ które zadaje agent
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


	// zmienne które u³atwi¹ nam identyfikacjê 
	int		ID;
	int		groupIndex;


	bool playerHurtedGroupMate, isRangeUnit, hasCharged;
	bool isGroupIntrigued;
	bool isGroupAgressive;

	// prêdkoœæ liniowa x i y
	sf::Vector2f V;

	// przyspieszenie 
	sf::Vector2f acceleration;

	// œrednie prêdkoœci ca³ej grupy 
	sf::Vector2f average;


	// zapisujemy sobie adresy parametrów które wrzucamy do update'a 
	sf::Vector2f *averageV;
	double		 *averageDistance;
	player		 *playerPointer;

	// je¿eli doszed³ do koñca planszy chcemy przyspieszyæ proces losowania ruchu
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

	// czas po którym mo¿e zostaæ znowu uderzony przez obszarówkê
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