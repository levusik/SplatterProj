#pragma once
#include "entity.hpp"
#include <SFML\Graphics.hpp>
#include "player.hpp"
#include "entity.hpp"
#include "enemy.hpp"
#include "button.h"
#include "terrainEffect.h"
#include <fstream>

class playGameState
{
public:
	playGameState(sf::Font &font, sf::RenderWindow &window);
	~playGameState();
	void run();

private:
	void			loadTemplatesAndItemsFromFile();
	void			initializeState();
	void			displayPlayingWaveState();
	void			updatePlayingWaveState();
	void			manageView();
	void			handleShooting();
	void			handleEnemies();
	void			updateGUI();
	void			generateSpawnPoints(int howManySpawnPoints);
	void			calculateBoidRules();
	void			handleCollisionDetection();
	void			moveImportantTexts(int vecX, int vecY);
	void			handleErasingProjectiles();
	void			roundValue(int size, int &value);
	bool			worthAddingToVec(std::string &line);
	void			manageAmmoAndCurrencyTexts();
	void			manageDeletingProjectiles(std::vector<projectile*>::iterator &iterator, int &counter);
	void			updateTerrainEffects();
	void			dealTerrainDamage();
	void			playDefeatAnimation();
	void			playVictoryAnimation();
	void			drawAllEnemies();
	void			generateRandomSpawnPoint(int &randX, int &randY);
	void			modifyDamageParameters(damageParameters &dmgParams);
	void			addWeaponToBackpack(int ID);

	
	double			calculateDistance(sf::Vector2f position1, sf::Vector2f position2);
	std::string		filtrateString(std::string &line);
	splatTemplate	getRandomSplatTemplate();


	sf::RenderWindow	&Window;
	sf::Font			&Font;

	// prostok¹t który pokazuje pole w którym gracz i przeciwnicy mog¹ siê poruszaæ
	sf::RectangleShape  moveAvailableRect;

	// zmienne prywatne 
	sf::Text	text;
	player		Player;
	sf::View	DynamicView, StaticView;
	sf::Clock   delayBtwAddingProjectiles;

	// zmienne do kontrolowania widoku
	double		xViewOffset, yViewOffset;
	int			SpawnPointIndex, howManyCreaturesLeft;
	int			allChance;

	bool lock = false;

	////////////////////////////////////////////////////////////////////////////////////////

	std::vector<sf::Vector2f>	averageV;
	std::vector<double>			averageDistance;
	std::vector<std::vector<std::vector<otherMatesParameters>>> positionsOfOtherBoids;

	sf::Vector2f positionOfNearestEnemy;

	////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	void	manageBackpack();

	// ekwipunek
	std::vector<rangeWeapon*>		backpack;
	std::map <typeOfWeapon, int>	Ammunitions;
	rangeWeapon*					currentlyWeapon;
	rangeWeapon*					currentlySelectedWeapon;
	projectile						Projectile;
	sf::Clock						delayBtwChangingWeapon;
	int								indexOfCurrentlySelectedWpn;
	playerParameters				PlayerParameters;

	/////////////////////////////////////////////////////////////////////////////


	splatTemplate				splatUnit;
	// wektor z pociskami
	std::vector<projectile*>	   ProjectilesArray;

	// wektor wektorów ze splatami
	std::vector<std::vector<enemy*>>  enemyArray;

	//wektor obszarówek
	std::vector<terrainEffect*>	terrainEffectsArray;


	std::vector <rangeWeapon*>   rangeWeaponDatabase;
	std::vector <splatTemplate> splatTemplateDatabase;


	// debug file 
	std::fstream debugFile;

	// zegar
	sf::Clock timeElapsed;

	// FSM
	statesOfPlayGameState currentState;

	/////////////////////////////////////////////////////////////////////////////////////////
	// GUI stuff

	int					  previousMagSize;
	typeOfWeapon		  prevWpnType;
	
	// Kolorki 
	std::map <typeOfWeapon, sf::Color> colorMap;

	std::vector<sf::Text> infoTexts;
	std::vector<sf::Text> ammoAndCurrencyTexts;

	sf::Clock			  timeElapsedWave;

	/////////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////////
	//		Setting Up Wave

	void	handleSettingUpWave();
	void	updateSettingUpWave();
	void	displaySettingUpWave();
	void	handleSettingUpWaveCounter();


	sf::Text  counter;
	sf::Clock SettingUpWaveTimeElapsed;

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	//			Ship state
	void	changeToCity();
	void	switchStateToShip();
	void	playAnimation();
	void	updateAnimation(sf::Color &col, bool managePlayer);
	void	initializeMainMenuShipState();


	void	handleShipState();
	void	drawShipState();
	void    updateShipState();


	void    handleButtons();
	void	interprateButtonSignals(usageOfButton &usage);
	void	interprateShopSignals(usageOfButton	&usage);
	void	switchState(statesOfShip switchToWhat);
	void	generateLineOfButtons(int howMany, int addToIter, sf::Vector2f size, sf::Vector2f StartingPos, sf::Color passiveColor, sf::Color interactedColor, bool generateInteractable);


	void	initializeShop();
	void	interprateMainMenuSignals(usageOfButton & usage);
	void	randomWeapon();
	void	handleBuyingWeapon(int index);
	void	handleBuyingAmmunition(typeOfWeapon typeOfAmmo, int howManyBlts);
	std::map<typeOfWeapon, int> costsOfBullets;



	std::vector  <rangeWeapon> weaponBuffor;
	std::vector <rangeWeapon> actualAssortiment;
	std::vector <int>		  weaponsIDNotTaken;


	void	intializePort();
	void	interpratePortSignals(usageOfButton &usage);

	void	initializeArena();
	void	interprateArenaSignals(usageOfButton &usage);

	void	intializeCabine();
	void	interprateCabineSignals(usageOfButton &usage);

	void	initializeStation();
	void	interprateStationSignals(usageOfButton &usage);



	Button				 *button;
	std::vector <Button>  buttons;
	statesOfShip		  inWhichRoomWeAre;
	sf::Text		      currencyText;
	sf::Clock			  delayBtwBuying;
	


	int currency;
	int alpha;
	double step;
	const int howManyButtons;
	bool	  isPlayingAnimation;
	sf::Clock timeOfAnimation;
	sf::Text animationText;
	std::map <typeOfWeapon, int> HowManyBltsInPacks;
	/////////////////////////////////////////////////////////////////////////////////////////
};