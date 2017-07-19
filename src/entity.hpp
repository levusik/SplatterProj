#pragma once
#include <SFML\Graphics.hpp>
#include <assert.h>


# define GETSEC(x) x.getElapsedTime().asSeconds()
#define radToDgr(x) ((x) * (M_PI / 180.))
#define cast(type,x) (static_cast<type>(x))
#define negate(x) x == 0 ? 1 : 0
#define min(x,y) (x) > (y) ? (y) : (x)
#define max(x,y) (x) > (y) ? (x) : (y)
#define getSign(x) (x) < 0 ? (-1) : (1)


/*****************************************************************/
/*			Zbiór wszystkich sta³ych							 */
const int    WINDOWWIDTH = 1280;
const int    WINDOWHEIGHT = 1024;
const int    FPS = 60;
const int    viewBoundsRectX = WINDOWWIDTH / 2;
const int    viewBoundsRectY = WINDOWHEIGHT / 2;
const int	 numOfValLoadedFromFile = 28;
const int	 DELAYBTWWAVES = 0;
const int	 LEFTWALL = 0;
const int	 UPWALL = 0;
const int    RIGHTWALL = 2000;
const int    DOWNWALL  = 2000;
const int	 SPAWNPOINTMARGIN = 100;
const int	 howManyWeaponsInAssortiment = 3;
const char   commentChar = '*';
const double terrainDmgLock = 1.f;
const double fleeDistancePrc = 1.25;
const double M_PI = 3.14159265359;

const double collidingEnemiesPith = 1.3;
const double minDistanceConst = 4.;
const double enemyChargeSpeedBuff = 1.2;
const double enemyChargeRadiusBuff = 1.1;


// sta³e do pocisków
const int	leadRangeOfRandomness		= 5;
const int	plasmaRangeOfRandomness		= 3;
const int	energeticRangeOfRandomness	= 10;
const float g_energeticSpeedBuff = 1.25;
const float g_plasmaSpeedBuff = 0.5;

// sta³a pozwalaj¹ca na zarz¹dzanie guziczkami na statku
const float  SPACER = 1 / 32.0;
/*****************************************************************/
/*					sta³e do Debugowania						 */
const bool   DEBUG = false;
const bool	 showMoveAvailableRect = true;



/*****************************************************************/
/*		Zbiór wszystkich enumów które u³atwi¹ nam ¿ycie i 
		dziêki nim kod bêdzie czytelniejszy						 */
enum class typesOfEnemyBehaviour
{
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
};	
enum class statesOfStateMachine
{
	mainMenu,
	loadGame,
	creatingCharacter,
	playingGame,
};
enum class stateOfEnemy
{
	IDLE,
	INTRIGUED,
	MAKINGDECISION,
	AGRESSIVE,
	CHARGE,
	ATTACK,
	FLEE,
	SPECIALABILITY
};
enum class statesOfPlayGameState
{
	SETTINGUPWAVE,
	PLAYINGWAVE,
	SHIP
};
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
	// guziki do obs³ugi main menu
	goToShop = 0,
	goToIntergalaxyPort = 1,
	goHome = 2,
	goToImplantationStation = 3,
	goToVirtualArena = 4,
	goFight = 5,
	goToMainMenu = 6,
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// guziki do obs³ugi sklepu
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
	// guziki do obs³ugi portu 
	TAKEQUEST_1 = 19,
	TAKEQUEST_2 = 20,
	TAKEQUEST_3 = 21,
	//////////////////////////////////////////////


	//////////////////////////////////////////////
	// guziki do obs³ugi Areny
	FIGHTBOSS_1 = 22,
	FIGHTBOSS_2 = 23,
	FIGHTBOSS_3 = 24,
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	// guziki do obs³ugi stacji implantacyjnej
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
enum class typeOfTerrainEffects
{
	VOID	  = 0,
	EXPLOSIVE = 1,
	FIRE	  = 2,
	TOXIC	  = 3,
	ELECTRIC  = 4
};
enum class typeOfSpecialAbility
{
	VOID,	  
	PASSIVE,  // pasywna modyfikuje lub nieco poprawia parametry gracza
	OFFENSIVE // zadaje obra¿enia 
};
/*****************************************************************/


/***************************************************************/
/*		Zbiór wszystkich struktur które bêdziemy przekazywaæ
		co u³atwi nam w doœæ du¿ym stopniu ¿ycie				*/
struct  terrainEffectParams
{
	terrainEffectParams()
	{
		type = typeOfTerrainEffects::VOID;
		this->step = 0;
		this->timeOfFunc = 0;
		this->function = NULL;
	}

	double damage, maxRadiusSize;
	double step;
	int	 timeOfFunc;
	double(*function)(double,int);
	typeOfTerrainEffects   type;
	double	 durationOfAnimation;
};
struct  effects
{
	effects()
	{
		this->blank();
	}
	void blank()
	{
		type =  typeOfWeapon::BULLETTYPE;
		a_manualTerrainDamageRadiusSize = INT_MIN;
		a_hasFlexibleBullets	= false;
		a_hasSmartBulletsBoolean= false;
		a_hasScope				= false;
		a_hasStabilizator		= false;
		a_hasMultiplierEffect	= false;
		a_StunEnenies			= false;
		a_hasExtendedMag		= false;
		a_bullThatBurns			= false;
		a_bullThatElectrize		= false;
		a_bullThatToxicate		= false;
		a_bullThatExplode		= false;
		l_hasGrenadeLauncher	= false;
		l_isFlameThrower		= false;
		p_plasmaMagicBullets	= false;
		p_preciseMultiShoot		= false;
		e_isCharger				= false;
		e_shootEverywhere		= false;
	}
	void createPlasmaEffcs(bool hasMagicBullets = 0, bool hasPreciseMultiShoot =0)
	{
		p_plasmaMagicBullets	=	 hasMagicBullets;
		p_preciseMultiShoot		=	 hasPreciseMultiShoot;

		if (hasPreciseMultiShoot)
			this->a_hasPackedBullets = true;
		else
			this->a_hasPackedBullets = false;

		l_hasGrenadeLauncher	= false;
		l_isFlameThrower		= false;

		e_isCharger				= false;
		e_shootEverywhere		= false;
	}
	void createLeadEffects(bool hasBulletsThatBurn= 0, bool hasBulletsThatElec = 0, bool hasBulletsThatToxicate = 0, bool hasGrenadeLauncher = 0, bool isFlameThrower = 0)
	{
		l_hasGrenadeLauncher = hasGrenadeLauncher;
		l_isFlameThrower = isFlameThrower;

		p_plasmaMagicBullets = false;
		p_preciseMultiShoot = false;

		e_isCharger = false;
		e_shootEverywhere = false;
	}
	void createEnergyEffects(bool isCharger =0, bool shootEverywhere = 0)
	{
		bool e_isCharger		= isCharger;
		bool e_shootEverywhere	= shootEverywhere;

		if (shootEverywhere)
			this->a_hasPackedBullets = true;
			
		l_hasGrenadeLauncher = false;
		l_isFlameThrower = false;

		p_plasmaMagicBullets = false;
		p_preciseMultiShoot = false;
	}

	typeOfWeapon type;
	
	// booleany dla wszystkich broni (ka¿da broñ mo¿e posiadaæ t¹ zdolnoœæ)
	bool a_hasFlexibleBullets;		// pociski odbijaj¹ siê od œcian
	bool a_hasSmartBulletsBoolean;	// czy ma "inteligentne pociski"
	bool a_hasPackedBullets;		// czy strzela kilka pociskami
	double a_manualTerrainDamageRadiusSize; // waroœæ która okreœli rozmiar obszarówki ( je¿eli wartoœæ bêdzie mniejsza od 0 to losujemy ze sta³ych)

	bool a_hasScope;			// + celnoœci
	bool a_hasStabilizator;		// mniejszy rozrzut 
	bool a_hasMultiplierEffect;	// kilka kulek na raz
	bool a_StunEnenies;			// zatrzymuje przeciwników
	bool a_hasExtendedMag;		// + do magazynka
	bool a_bullThatBurns;		// obra¿enia od ognia
	bool a_bullThatElectrize;	// obra¿enia od elektrycznoœci
	bool a_bullThatToxicate;	// obra¿enia od toksycznoœci
	bool a_bullThatExplode;		// wybuchaj¹ce pociski


	//  tylko dla broni o³owiowych
	bool l_hasGrenadeLauncher;	// pytanie czy ma granatnik (alternatywny atak)
	bool l_isFlameThrower;		// czy jest miotaczem ognia ( obra¿enia od ognia + zwrotna szybkostrzelnoœæ + bardzo ma³e obra¿enia)
	


	// tylko dla plazmowych
	bool p_plasmaMagicBullets;	// niesamowite pociski plazmowe
	bool p_preciseMultiShoot;		// broñ wypluwa kilka-kilkanaœcie pocisków które lec¹ bardzo blisko siebie


	// tylko dla energetycznych
	bool e_isCharger;			// nieskoñczonoœæ amunicji ale bardzo wolne prze³adowania 
	bool e_shootEverywhere;		// wysy³a pocisk w ka¿dym mo¿liwym kierunku
};
struct  splatTemplate
{
public:
	typesOfEnemyBehaviour enemyBehaviour;
	sf::Vector2f maxSpeed;
	sf::Vector2f speedVec;
	sf::Vector2f acceleration;
	double circleRadius;
	int minHP, maxHP;
	int minDamage, maxDamage;
	int minDefense, maxDefense;
	int minAgility, maxAgility; //	ma wp³yw na szybkoœæ oraz tempo zmian ruchu 
	int viewSize;				// dystans po którym dostrzega przeciwnika 
	int worth;					// ile kasy za niego dajemy


								// interakcja z ziomkami z grupy i graczem
	double reactionTime;		// czas po którym agent reaguje na czynniki zewnêtrzne 
	int safeDistance;			// dystans po którym, je¿eli przekroczony agent zastanawia siê co zrobiæ
	int miniumDistance;			// dystans po którym, je¿eli przekroczony agent zaczyna walczyæ ( nie wa¿ne w jakim jest stanie)
	int fleeDistance;			// po jakim dystansie agent przechodzi ze stanu ucieczki do "normalnoœci"
	int minDistanceFromMates;	// minimalny dystans od ziomków z grupy
	int chargeDistance;			// dystans po którym, je¿eli przekroczony agent zaczyna szar¿owaæ ()
	int AttackRange;			// zasiêg broni
	int minGroupSize,
		maxGroupSize;


								// waga regu³ algorytmu
	double  pith;


	// czy jest jednostk¹ strzelaj¹c¹ 
	bool isShootingUnit;
	int firerate;
	int projectileRadius;


	int		chanceOfGettingRandomized;


	sf::Vector2f startingPosition;

	// kolorki !
	sf::Color agressiveModeColor;
	sf::Color reactingtoPlayerColor;
	sf::Color ChargingColor;
	sf::Color fleeingColor;
	sf::Color attackingPlayerColor;
	sf::Color passiveModeColor;
};

struct  otherMatesParameters
{
	otherMatesParameters()
	{
		this->currentState = stateOfEnemy::IDLE;
		this->hasDetectedPlayer = false;
	}
	double radius;
	sf::Vector2f	 position;
	stateOfEnemy currentState;
	bool	hasDetectedPlayer;
};
struct  playerParameters
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
		this->maxHP = 0;
	}
	// podstawowe parametry

	// bardziej przyziemne rzeczy do obs³ugi broni
	double	maxHP;
	double	angle;
	sf::Vector2f position;

	// uderzenie krytyczne
	double criticalShotMultipler;
	int criticalShotChance;


	// szybkoœæ prze³adowania, szybkoœæ poruszania i szybkoœæ machania broni¹ do w.w. 
	int agility;

	// celnoœæ strza³u 
	int perception;

	// si³a ( multipler damage'u)
	int strength;

	// bêdzie mieæ wp³yw na ceny w sklepie
	int rethoric;



	//////////////////////////////////////////////////////////////////////////////////////
	// pomys³y na Perki 

	// szybkie prze³adowania
	bool fastHands;

	// sniper perk ( dziêki temu perkowi gracz mo¿e poprzez PPM zredukowaæ rozrzut, zmiejszyæ prêdkoœæ i zwiêkszyæ celnoœæ)
	bool sniperPerk;



	//////////////////////////////////////////////////////////////////////////////////////
};
struct  weaponParameters
{
	weaponParameters()
	{
		this->name = "None";
		this->weaponShopCost = 0;
		this->ID = 0;
		this->maxDamage = 0;
		this->minDamage = 0;
		this->bulletSize = 0;
		this->recoil = 0;
		this->firerate = 0;
		this->minSpeedOfBullet = 0;
		this->maxSpeedOfBullet = 0;
		this->weaponEffects.blank();
		this->origMagSize = 0;
		this->actualMagSize = 0;
		this->howManyProjItShoot = 1;
		this->costOfBullet = 1;
		this->reloadingTime = 1;
	}
	std::string name;
	int weaponShopCost;

	int		ID;
	double  maxDamage, minDamage, bulletSize, recoil, firerate;
	double  reloadingTime;
	double  minSpeedOfBullet, maxSpeedOfBullet;
	int		howManyProjItShoot;
	int		costOfBullet;
	int		origMagSize;
	int		actualMagSize;
	bool	packedBullets; // infomracja czy pociski którymi broñ pluje s¹ "upakowane"
						   // je¿eli s¹ upakowane to niezale¿nie od tego jak wiele pocisków 
						   // leci to zabierze tylko 1 nabój z magazynka ( w przeciwnym razie 
						   // zabierze tyle ile strzela ).
	effects weaponEffects;
};
struct  damageParameters {
	damageParameters()
	{
		this->value = 0;
	}
	double value;
};
struct  bulletParameters {


	effects Effects;
	double damage;
};

/***************************************************************/


class projectile;

/*****************************************************************/
/*						Deklaracje klas							 */
class rangeWeapon
{
public:
	// domyœlny konstruktor czy broñ blank 
	rangeWeapon();
	void clear();

	virtual void create(weaponParameters params);
	virtual void setEffects(effects efcsCausedByWpn);
	virtual void initializeEffects();
	virtual void tryReloading(std::map<typeOfWeapon, int> &Ammunitions);
	virtual projectile* manageAddingSpecialBullets(playerParameters playerParams, int id);
	virtual void handleReloading(std::map<typeOfWeapon, int> &Ammunitions, playerParameters playerParams);
	virtual void takeAction(std::map<typeOfWeapon, int> &Ammunitions, std::vector<projectile*> &ProjectilesArray, playerParameters playerParams);
	virtual void manageShooting(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile*>& ProjectilesArray, playerParameters playerParams);
	virtual void managePassingFunctionsToBullet(projectile *proj);


	bool hasSmartBullets();
	int howManyBulletsLeftInMag();
	int getMaxMagSize();

	typeOfWeapon getType();


	int		ID;
	double(*funcPtr)(double);
	double  maxDamage, minDamage, bulletSize, recoil, firerate;
	double  minSpeedOfBullet, maxSpeedOfBullet;
	effects EffectWeaponCauses;


protected:

	int			origMagSize;
	int			actualMagSize;
	int			howManyProjItShoot;
	int			reloadingTime;
	bool		isAutomatic;
	int			costOfBullet;

	bool		SAFAYCC;					// Shoot As Fast As You Can Click ( jakieœ rewolwery czy inne cuda)
	bool		packedBullets;				// infomracja czy pociski którymi broñ pluje s¹ "upakowane"
											// je¿eli s¹ upakowane to niezale¿nie od tego jak wiele pocisków 
											// leci to zabierze tylko 1 nabój z magazynka ( w przeciwnym razie 
											// zabierze tyle ile strzela ).
	bool		isRecharger;				// ustawia pojemnoœæ magazynka na 999 i nieskoñczon¹ iloœæ amunicji 
											// jednak ma bardzo du¿y czas prze³adowania.
	bool		hasFlexibleBullets;			// pociski odbijaj¹ siê od œcian
	bool		hasSmartBulletsBoolean;		// "inteligentne" pociski które lec¹ w kierunku przeciwnika
	sf::Vector2f bulletDestination;			// potrzebne do "inteligentnych pocisków"


	bool		SAFAYCCLock;
	bool		isReloading;


	sf::Clock   timeElpasedForReloading;
	sf::Clock	delayBtwAutoReloading;
	sf::Clock	timeElapsedFirerate;

};
class projectile : public sf::CircleShape
{
public:
	projectile();
	~projectile();
	
	void setFunctionPtr( double(*ptr)(double), double step);
	virtual void create(sf::Vector2f playerPos, double radians, rangeWeapon *weapon, int iter);
	
	
	virtual void update();
	

	// metoda potzebna do smart bullet 
	void setSmartBulletDestination(sf::Vector2f position);
	


	// zarz¹dzanie efektami 
	virtual void manageEffectsInit(rangeWeapon weapon, double radians, int iter);
	virtual void managePlasmaEffectsinit();
	virtual void manageEnergeticEffectsInit();
	virtual void manageBulletTypeEffectsInit();
	virtual void handleCommonEffects(rangeWeapon weapon);
	virtual void manageSpecialEffects() {}
	virtual void initializeSpecialEffects() {}

	// animacje podczas update'u
	virtual void handleEnergeticAnimations();
	virtual void handlePlasmaAnimations();

	// gettery
	bool					needToDelete();
	virtual bool			isSmartBullet();
	virtual bool			isFlexibleBullet();
	double					getDmg();
	double					getRandomVal(int minVal, int maxVal);
	double					getMaxTime();
	sf::Vector2f			getCenter();
	effects					getEffects();
	sf::Time				getTime();
	terrainEffectParams 	getTerrainEffects();
	damageParameters		getDamageParameters();


protected:
	void intializeTerrainEffects(double dmgDebuff, int maxTime, int minTime, double(*funcPtr)(double, int), double dmgMultipler, int step, typeOfTerrainEffects type);

	
	// function ptr
	double (*func)(double);
	double funcCounter,funcStep;



	int colorCounter;
	double newRadius;
	double prevRadiusSize;
	bool NeedToDeleteBoolean;
	double animCounter, step;
	int origRadiusSize, actualOrigRadiusSize;
	double maxTimeOfProjection, damage, Vx, Vy, recoilValue, PartOfDrawnCircle;
	

	bool   isSmartBulletBoolean;
	bool   needToDrawLine;
	effects *EfctsCausedByBlt;
	sf::Clock timeOfProjection;
	sf::Clock timeBasedMovement;

	terrainEffectParams TerrainEffectsParameters;

	sf::Vector2f bulletDestination;

};
class aliveCreature
{
public:
	aliveCreature()
	{
		this->isThisAlive = true;
	}
	bool isAlive() const
	{
		return this->isThisAlive;
	}
	double getHP() const
	{
		return this->HP;
	}
	virtual void dealDamage(damageParameters dmgParams)
	{
		this->HP -= dmgParams.value;

		if (HP <= 0)
		{
			this->isThisAlive = false;
		}
	}
protected:

	double HP;
	bool isThisAlive;
};
/*****************************************************************/

