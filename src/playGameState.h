#pragma once
#include "entity.hpp"
#include <SFML\Graphics.hpp>
#include "player.hpp"
#include "entity.hpp"
#include "enemy.hpp"
#include "button.h"
#include <fstream>


class playGameState
{
public:
	playGameState(sf::Font &font, sf::RenderWindow &window);
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
	void			generateSpawnPoint(int howManySpawnPoints);
	void			calculateBoidRules();
	void			handleCollisionDetection();
	void			moveImportantTexts(int vecX, int vecY);
	void			handleErasingProjectiles();
	void			roundValue(int size, int &value);
	bool			worthAddingToVec(std::string &line);
	double			calculateDistance(Verticle position1, Verticle position2);
	std::string		filtrateString(std::string &line);
	splatTemplate	getRandomSplatTemplate();
	void			manageAmmoAndCurrencyTexts();


	sf::RenderWindow &Window;
	sf::Font		 &Font;


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

	std::vector<Verticle>	averageV;
	std::vector<double>	averageDistance;
	std::vector<std::vector<std::vector<otherMatesParameters>>> positionsOfOtherBoids;


	////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	void	manageBackpack();

	// ekwipunek
	std::vector<rangeWeapon>		backpack;
	std::map <typeOfWeapon, int>	Ammunitions;
	rangeWeapon						currentlyWeapon;
	rangeWeapon						currentlySelectedWeapon;
	projectile						Projectile;
	sf::Clock						delayBtwChangingWeapon;
	int								indexOfCurrentlySelectedWpn;
	playerParameters				PlayerParameters;

	/////////////////////////////////////////////////////////////////////////////


	splatTemplate				splatUnit;
	// wektor z pociskami
	std::vector<projectile>		   ProjectilesArray;

	// wektor wektorów ze splatami
	std::vector<std::vector<splat>>  splatArray;
	std::vector <splatTemplate> splatTemplateDatabase;
	std::vector <rangeWeapon>   rangeWeaponDatabase;


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
	void	playAnimation();
	void	updateAnimation(bool managePlayer);
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

	void	intializePort();
	void	interpratePortSignals(usageOfButton &usage);

	void	initializeArena();
	void	interprateArenaSignals(usageOfButton &usage);

	void	intializeCabine();
	void	interprateCabineSignals(usageOfButton &usage);

	void	initializeStation();
	void	interprateStationSignals(usageOfButton &usage);



	Button	*button;
	std::vector <Button> buttons;
	statesOfShip inWhichRoomWeAre;

	int currency;
	int alpha;
	double step;
	const int howManyButtons;
	bool	  isPlayingAnimation;
	sf::Clock timeOfAnimation;
	sf::Text victoryText;
	/////////////////////////////////////////////////////////////////////////////////////////
};