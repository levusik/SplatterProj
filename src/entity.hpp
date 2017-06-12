#pragma once
#include <SFML\Graphics.hpp>
#include <assert.h>

const int    WINDOWWIDTH = 1024;
const int    WINDOWHEIGHT = 768;
const int    FPS = 60; 
const double M_PI = 3.14159265359;
const int    viewBoundsRectX = WINDOWWIDTH / 2;
const int    viewBoundsRectY = WINDOWHEIGHT / 2;
const bool   DEBUG = false;
const char   commentChar = '*';
const int	 numOfValLoadedFromFile = 36;
const int	 DELAYBTWWAVES = 5;
const int	 LEFTWALL = 0;
const int	 UPWALL = 0;
const int    RIGHTWALL = 200;
const int    DOWNWALL = 200;


// sta�e do pocisk�w
const float g_energeticSpeedBuff = 1.25;
const float g_plasmaSpeedBuff = 0.6;


// sta�a pozwalaj�ca na zarz�dzanie guziczkami na statku
const float  SPACER = 1 / 32.0;


enum class statesOfStateMachine
{
	mainMenu,
	loadGame,
	creatingCharacter,
	playingGame,
};
enum class stateOfEnemy
{
	WALKING_ARROUND,
	REACT_TO_PLAYER,
	MOVING_TOWARDS_PLAYER,
	ATTACK_PLAYER,
	FLEE,
};
enum class statesOfPlayGameState
{
	SETTINGUPWAVE,
	PLAYINGWAVE,
	SHIP
};

// enum kt�ry przechowuje warto�ci kt�re, je�eli guzik.update() zwr�ci 1
// b�dziemy odpowiednio interpetowa� ( je�eli klikni�to guzik z przeznaczeniem goToShop 
// to uruchomimy funkcj� udpowiadaj�c� za obs�ug� sklepu)
//	* Sklep					 ( zakup broni )
//	* port mi�dzygalaktyczny ( questy ? )
//	* kabina				 ( rozw�j postaci)
//  * stacja implantacyjna   ( powerupy i wzmocnienia )
//  * arena wirtualna		 ( przetestowanie bronii, powerup�w, walka z bossami)
//	* walka					 ( nast�pna fala )

// stany do FSM'a do statku
enum class statesOfShip
{
	MAINMENU = 0,
	SHOP = 1,
	INTERGALAXYPORT = 2,
	IMPLANTATIONSTATION = 3,
	VIRTUALARENA = 4,
	FIGHT = 5
};
enum class usageOfButton
{
	//////////////////////////////////////////////
	// guziki do obs�ugi main menu
	goToShop = 0,
	goToIntergalaxyPort = 1,
	goHome = 2,
	goToImplantationStation = 3,
	goToVirtualArena = 4,
	goFight = 5,
	goToMainMenu = 6,
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// guziki do obs�ugi sklepu
	BUYWEAPON1 = 7,
	BUYWEAPON2 = 8,
	BUYWEAPON3 = 9,

	BUYAMMOPACK1_1 = 10,
	BUYAMMOPACK2_1 = 11,
	BUYAMMOPACK3_1 = 12,

	BUYAMMOPACK1_2 = 13,
	BUYAMMOPACK2_2 = 14,
	BUYAMMOPACK3_2 = 15,

	BUYAMMOPACK1_3 = 16,
	BUYAMMOPACK2_3 = 17,
	BUYAMMOPACK3_3 = 18,
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// guziki do obs�ugi portu 
	TAKEQUEST_1 = 19,
	TAKEQUEST_2 = 20,
	TAKEQUEST_3 = 21,
	//////////////////////////////////////////////


	//////////////////////////////////////////////
	// guziki do obs�ugi Areny
	FIGHTBOSS_1 = 22,
	FIGHTBOSS_2 = 23,
	FIGHTBOSS_3 = 24,
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// guziki do obs�ugi stacji implantacyjnej
	CHOOSEDIMPLANT_1 = 25,
	CHOOSEDIMPLANT_2 = 26,
	CHOOSEDIMPLANT_3 = 27,
	CHOOSEDIMPLANT_4 = 28,

	CHOOSEDIMPLANT_5 = 29,
	CHOOSEDIMPLANT_6 = 30,
	CHOOSEDIMPLANT_7 = 31,
	CHOOSEDIMPLANT_8 = 32,
	//////////////////////////////////////////////


	_NULL
};
enum class typeOfWeapon
{
	BULLETTYPE,
	ENERGETIC,
	PLASMA,
	_NULL
};

class projectile;

// struktura kt�ra zawiera wszystkie potrzebne zmienne do Orkasa. 
// wczytamy to sobie elegancko z pliku po czym przeka�emy do klasy ork 
// kt�ra odczyta sobie wszystkie parametry ze struktury

struct effects
{
	effects()
	{
		this->blank();
	}
	void blank()
	{
		// defaultowy set efekt�w:
		// nudny o�owiowy karabin
		this->e_energeticTerrainDamage = 0;
		this->l_hasScope = 0;
		this->s_laserEffect = 0;
		this->e_multiplierEffect = 0;
		this->l_bullThatBurns = 0;
		this->l_bullThatElectrize = 0;
		this->l_bullThatToxicate = 0;
		this->a_hasStabilization = 0;
		this->p_plasmaMagicBullets = 0;
		this->type = typeOfWeapon::BULLETTYPE;
	}

	typeOfWeapon type;
	bool e_energeticTerrainDamage;
	bool l_hasScope;
	bool s_laserEffect;
	bool e_multiplierEffect;
	bool l_bullThatBurns;
	bool l_bullThatElectrize;
	bool l_bullThatToxicate;
	bool a_hasStabilization;
	bool p_plasmaMagicBullets;
};
struct splatTemplate
{
public:
	// dajemy warto�ci z zakresu dla urozmaicenia rozgrywki 
	// (czasem ork ma wi�kszy pancerz, czasem wi�kszy damage itp.)
	double	minArmor, maxArmor, minDamage, maxDamage, minHP, maxHP,
			rangeOfWeapon, minDistanceFromMates;
	double	minSpeed, maxSpeed, timeToMakeDecision, acceleration;
	double	sizeOfView, angleOfView, pith, timeToMakeDecisionInSec;
	float	changeMoveBy;
	float	chargeDistance;
	double	safeDistance;
	double	radius;
	int		chanceOfGettingRandomized;
	int		value;


	sf::Vector2f startingPosition;

	// kolorki !
	sf::Color agressiveModeColor;
	sf::Color reactingtoPlayerColor;
	sf::Color ChargingColor;
	sf::Color fleeingColor;
	sf::Color attackingPlayerColor;
	sf::Color passiveModeColor;
};
struct Verticle
{
	void blank()
	{
		// metoda kt�ra wyzeruje wszystkie zmienne
		this->x = 0;
		this->y = 0;
	}

	double x, y;
};
struct otherMatesParameters
{
	Verticle position;
	bool	hasDetectedPlayer;
};
struct playerParameters
{
	playerParameters()
	{
		this->angle = 0;
		this->position.x = 0;
		this->position.y = 0;
		this->agility = 0;
		this->perception = 0;
		this->strength = 0;
		this->rethoric = 0;
		this->fastHands = 0;
		this->sniperPerk = 0;
	}

	// bardziej przyziemne rzeczy do obs�ugi broni
	double angle;
	sf::Vector2f position;


	// podstawowe parametry

	// szybko�� prze�adowania, szybko�� poruszania i szybko�� machania broni� do w.w. 
	int agility;

	// celno�� strza�u 
	int perception;

	// si�a ( multipler damage'u)
	int strength;

	// b�dzie mie� wp�yw na ceny w sklepie
	int rethoric;



	//////////////////////////////////////////////////////////////////////////////////////
	// pomys�y na Perki 

	// szybkie prze�adowania
	bool fastHands;

	// sniper perk ( dzi�ki temu perkowi gracz mo�e poprzez PPM zredukowa� rozrzut, zmiejszy� pr�dko�� i zwi�kszy� celno��)
	bool sniperPerk;



	//////////////////////////////////////////////////////////////////////////////////////
};
struct weaponParameters
{
	weaponParameters()
	{
		this->ID = 0;
		this->maxDamage = 0;
		this->minDamage = 0;
		this->bulletSize = 0;
		this->recoil = 0;
		this->firerate = 0;
		this->minSpeedOfBullet = 0;
		this->maxSpeedOfBullet = 0;
		this->EffectWeaponCauses.blank();
		this->origMagSize = 0;
		this->actualMagSize = 0;
		this->howManyProjItShoot = 0;
		this->isAutomatic = 0;
		this->SAFAYCC = 0;
		this->costOfBullet = 1;
	}
	int		ID;
	double  maxDamage, minDamage, bulletSize, recoil, firerate;
	double  minSpeedOfBullet, maxSpeedOfBullet;
	effects EffectWeaponCauses;
	int		origMagSize;
	int		actualMagSize;
	int		howManyProjItShoot;
	bool    isAutomatic;
	bool	SAFAYCC;	// Shoot As Fast As You Can Click ( jakie� rewolwery czy inne cuda)
	int		costOfBullet;
};
class rangeWeapon
{
public:
	// domy�lny konstruktor czy bro� blank 
	rangeWeapon();
	void clear();
	void setEffects(effects efcsCausedByWpn);
	void takeAction(std::map<typeOfWeapon, int> &Ammunitions, std::vector<projectile> &ProjectilesArray, playerParameters playerParams);
	void tryReloading(std::map<typeOfWeapon, int> &Ammunitions);
	void create(double minDamage, double maxDamage, double bulletSize, double recoil, double firerate, double minSpeedOfBullet, double maxSpeedOfBullet);
	void create(weaponParameters params);
	void manageShooting(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile>& ProjectilesArray, playerParameters playerParams);
	int howManyBulletsLeftInMag();
	int getMaxMagSize();

	typeOfWeapon getType();

	int		ID;
	double  maxDamage, minDamage, bulletSize, recoil, firerate;
	double  minSpeedOfBullet, maxSpeedOfBullet;
	effects EffectWeaponCauses;

private:

	int			origMagSize;
	int			actualMagSize;
	int			howManyProjItShoot;
	int			reloadingTime;
	bool		isAutomatic;
	int			costOfBullet;
	bool		SAFAYCC;	// Shoot As Fast As You Can Click ( jakie� rewolwery czy inne cuda)

	bool		SAFAYCCLock;
	bool		isReloading;
	sf::Clock	delayBtwReloading;
	sf::Clock   timeElpasedForReloading;
	sf::Clock	timeElapsedFirerate;

};
class projectile : public sf::CircleShape
{
public:
	projectile();
	void create(sf::Vector2f playerPos, double angle, rangeWeapon *weapon);
	void update();
	double getRandomVal(int minVal, int maxVal);
	Verticle getCenter();


	// zarz�dzanie efektami 
	void manageEffectsInit(rangeWeapon weapon, double angle);
	void managePlasmaEffectsinit();
	void manageEnergeticEffectsInit();
	void manageBulletTypeEffectsInit();
	void handleCommonEffects(rangeWeapon weapon);

	// animacje podczas update'u
	void handleEnergeticAnimations();
	void handlePlasmaAnimations();

	// gettery
	effects  getEffects();
	double   getDmg();
	sf::Time getTime();
	double	 getMaxTime();


private:
	double animCounter, step;
	int origRadiusSize, actualOrigRadiusSize;
	double maxTimeOfProjection, damage, Vx, Vy, recoilValue, PartOfDrawnCircle;
	int colorCounter;

	sf::Clock timeOfProjection;
	bool   needToDrawLine;
	effects *EfctsCausedByBlt;

};
struct bulletParameters {

	bulletParameters operator=(projectile bullet)
	{
		this->damage = bullet.getDmg();
		this->Effects = bullet.getEffects();
		return *this;
	}
	effects Effects;
	double damage;
};

/*

TODO:

***************************************************
BRONIE:

- implementacja sensownego systemu r�norodnych broni.
- Algorytm losowania r�nych broni w sklepie.
- ka�da bro� b�dzie posiada� jakie� atrybuty kt�re wywo�uj� jakie� efekty.

Plany na efekty:
- mamy 3 rodzaje broni :

#############################################
energetyczna

ma�y dmg, wysoka szybostrzelno��,
du�y rozrzut(zwi�ksza si� po czasie),
mniej efekt�w, du�o amunicji i do walki na czas (efekty spalania)

specjalno�ci:
- wy�adowania energetyczne (przechodzi na innych przeciwnik�w z grupy (to b�dzie ciekawe do zrobienia))
- obszar�wka	( raczej �atwe)
- bajery do zwi�kszenia celno�ci i szybkostrzelno�ci
- �adowarka	( nieograniczona amunicja ale bardzo wolne prze�adowania (to b�dzie ciekawe z innymi bro�mi :D ))
- laser		( ci�g�a wi�zka kt�ra rani (to te� b�dzie ciekawe :D ))
- rozdzielacz	( kilka wi�zek )

#############################################
o�owiowa
�redni damage, �rednia szybkostrzelno��, rozs�dny rorzut,
rozs�dna celno��, praktycznie brak efekt�w,
sensowna ilo�� amunicji i w sumie dla ka�dego

specjalno�ci:
- celownik (wi�ksza celno��)
- amunicja zapalaj�ca, elektryzuj�ca, toksyczna, wybuchowa
- jakie� bajery w stylu stablizator, kolba i inne szity
- podwieszany laser ( u�atwia celowanie  )
- granatnik (?)

#############################################

plazmowa
wyklepany damage, beznadziejna szybkostrzelno��,
wyklepane efekty, tragiczny rozrzut, tragiczna celno��,
malutka ilo�� amunicji , Do walki przeciwko ci�szym przeciwnikom i bossom

specjalno�ci:

- pociski rosn� po czasie by wybuchn��
- pociski lec� w ka�dym kierunku
- miotacz plazmy
- nak�ada na splata specjalny efekt kt�ry je�eli splat umrze to splaty obok niego otrzymuj� damage ( to b�dzie ciekawe)
-


Efekty broni Pomys�y:
- obszar�wki (granatnik, miotacz ognia, minigun)
- modyfikatory Celno�ci
- namierz�j�ce pociski (lec� w kierunku wroga)
- rozmiary magazynk�w
- kasetowe bronie
- wybuchaj�ca amunicja
- p�omienne pociski
- elektryczne pociski
- kwasowe pociski
- automatic, singleShot
- doubleShot, tripleShot itp.
*/