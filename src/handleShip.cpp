#include "playGameState.h"
#include <Windows.h>
#define getWx(window) (this->window.getSize().x) 
#define getWy(window) (this->window.getSize().y)


///////////////////////////////////////////////////////////////////////////////////////////////
//
//			Obs³uga Statku 
//	* Sklep					 ( zakup broni )
//	* port miêdzygalaktyczny ( questy ? )
//	* kabina				 ( rozwój postaci)
//  * stacja implantacyjna   ( powerupy i wzmocnienia )
//  * arena wirtualna		 ( przetestowanie bronii, powerupów, walka z bossami)
//	* walka					 ( nastêpna fala )

void playGameState::initializeMainMenuShipState()
{
	Window.setView(StaticView);
	buttons.clear();

	assert(SPACER <= 0.25);
	float sizeOfButton = ((1.0 - 4.0 * SPACER) / 3.0) * getWx(Window);

	for (int i = 0; i < this->howManyButtons; ++i)
	{
		this->button = new Button(static_cast<usageOfButton>(i),
			sf::Vector2f(sizeOfButton, sizeOfButton),
			sf::Vector2f((i % 3) * sizeOfButton + getWx(Window)*SPACER + (i % 3)*getWx(Window)*SPACER,
			(i / 3) * sizeOfButton + getWx(Window)*SPACER + (i / 3)*getWx(Window)*SPACER),true);
		this->button->setColors(sf::Color::Color(0xff, 0x0, 0x0), sf::Color::Color(0x60, 0xBF, 0x20));
		this->buttons.push_back(*button);
		delete button;
	}


	button = NULL;

	this->currencyText.setString(std::to_string(this->currency) + " $");
	this->currencyText.setFont(this->Font);
	this->currencyText.setCharacterSize(96);
	this->currencyText.setPosition(SPACER * this->Window.getSize().x, this->Window.getSize().y- this->currencyText.getCharacterSize() - SPACER * this->Window.getSize().y);
	this->currencyText.setFillColor(sf::Color::Color(0xda, 0xa5, 0x20));

}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleShipState()
{
	// (!) to tutaj jest bezsensu (do poprawy)
	if (!this->buttons.size())
	{
		// ma³y delay 
		Sleep(50);
		this->initializeMainMenuShipState();
	}

	updateShipState();
	drawShipState();

}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::drawShipState()
{
	this->Window.clear();

	for (int i = 0; i < buttons.size(); ++i)
	{
		this->buttons[i].draw(this->Window);
	}
	if (this->inWhichRoomWeAre != statesOfShip::FIGHT && this->inWhichRoomWeAre != statesOfShip::MAINMENU)
	{
		this->Window.draw(this->currencyText);
	}
	this->Window.display();
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::updateShipState()
{
	sf::Event event;
	while (this->Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			exit(0);
		}
	}

	// metoda która zajmie siê obs³ug¹ guzików oraz sygna³ów które "emituj¹"
	handleButtons();


}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleButtons()
{

	usageOfButton usageOfPressedButton;
	usageOfPressedButton = usageOfButton::_NULL;
	for (int i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i].update(this->Window))
		{
			usageOfPressedButton = buttons[i].getUsage();
			break;
		}
	}
	interprateButtonSignals(usageOfPressedButton);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateButtonSignals(usageOfButton & usage)
{
	// dla kilka stanów powtarza siê ta sama kontrolka po cholerê powtarzaæ kod
	if (usage == usageOfButton::goToMainMenu)
	{
		switchState(statesOfShip::MAINMENU);
		return;
	}

	switch (this->inWhichRoomWeAre)
	{
	case statesOfShip::MAINMENU:
		interprateMainMenuSignals(usage);
		break;

	case statesOfShip::SHOP:
		interprateShopSignals(usage);
		break;

	case statesOfShip::INTERGALAXYPORT:
		interpratePortSignals(usage);
		break;

	case statesOfShip::IMPLANTATIONSTATION:
		interprateStationSignals(usage);
		break;

	case statesOfShip::VIRTUALARENA:
		interprateArenaSignals(usage);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateMainMenuSignals(usageOfButton & usage)
{
	switch (usage)
	{
	case usageOfButton::goFight:
	{
		switchState(statesOfShip::FIGHT);
		return;
	}
	case usageOfButton::goHome:
	{
		// (!)
		MessageBox(NULL, "odpalam kabinê", NULL, NULL);
		break;
	}
	case usageOfButton::goToIntergalaxyPort:
	{
		switchState(statesOfShip::INTERGALAXYPORT);
		break;
	}
	case usageOfButton::goToImplantationStation:
	{
		switchState(statesOfShip::IMPLANTATIONSTATION);
		break;
	}
	case usageOfButton::goToShop:
	{
		switchState(statesOfShip::SHOP);
		break;
	}
	case usageOfButton::goToVirtualArena:
	{
		switchState(statesOfShip::VIRTUALARENA);
		break;
	case usageOfButton::goToMainMenu:
	{
		switchState(statesOfShip::MAINMENU);
		break;
	}
	default:
		// ignorujemy gdy wyst¹pi coœ innego
		return;
	}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleBuyingAmmunition(typeOfWeapon typeOfAmmo, int howManyBlts)
{
	if (delayBtwBuying.getElapsedTime().asSeconds() > 0.2f)
	{
		if (costsOfBullets[typeOfAmmo] * howManyBlts <= this->currency)
		{
			Ammunitions[typeOfAmmo] += howManyBlts;
			this->currency -= howManyBlts * costsOfBullets[typeOfAmmo];
		}
		else
		{
			// (!) beep alarm 
			MessageBox(NULL, "Brak amunicji!", NULL, NULL);
		}
		this->currencyText.setString(std::to_string(this->currency) + " $");
		this->delayBtwBuying.restart();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleBuyingWeapon(int index)
{
	if (index == 0 && actualAssortiment.size() > 0)
	{
		addWeaponToBackpack(actualAssortiment[index].ID);
	}
	else if (index == 1 && actualAssortiment.size() > 1)
	{
		addWeaponToBackpack(actualAssortiment[index].ID);
	}
	else if (index == 2 && actualAssortiment.size() > 2)
	{
		addWeaponToBackpack(actualAssortiment[index].ID);
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::randomWeapon()
{
	actualAssortiment.clear();

	// zamys³ : chcemy by dodaæ do bufora te wszystkie bronie które nie zosta³y jeszcze wziête
	// przez gracza i z tej kolejki bêdziemy wybieraæ sobie bronie do aktualnego asortymentu



	if (!weaponBuffor.size())
		for (int i = 0; i < weaponsIDNotTaken.size(); ++i)
		{
			weaponBuffor.push_back(*rangeWeaponDatabase[weaponsIDNotTaken[i]]);
			
			if (DEBUG)
			{
				std::cout << "wrzucam na bufor bron : \n";
				std::cout << rangeWeaponDatabase[weaponsIDNotTaken[i]]->ID << "\n";
				std::cout << rangeWeaponDatabase[weaponsIDNotTaken[i]]->bulletSize << "\n";
				std::cout << rangeWeaponDatabase[weaponsIDNotTaken[i]]->firerate << "\n\n";
			}
		}

	if (DEBUG)
	{
		std::cout << "w buforze znajduj¹ siê aktualnie : \n";

		for (int i = 0; i < weaponBuffor.size(); ++i)
		{
			std::cout << weaponBuffor[i].ID << "\n";
		}
	}

	for (int i = 0; i <  std::min(cast(int,weaponBuffor.size()),howManyWeaponsInAssortiment); ++i)
	{
		// wylosowanie broni z bufora
		int randomIndex = rand() % weaponBuffor.size();
		rangeWeapon Weapon = weaponBuffor[randomIndex];
		
		// usuniêcie broni z bufora
		std::vector<rangeWeapon>::iterator weaponBuffIter = weaponBuffor.begin();
		weaponBuffIter += randomIndex;
		weaponBuffor.erase(weaponBuffIter);

		actualAssortiment.push_back(Weapon);
	}

	if (DEBUG)
	{
		for (int i = 0; i < actualAssortiment.size(); ++i)
			std::cout << "wylosowane bronie to " << actualAssortiment[i].ID << "\n";
	}

}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateShopSignals(usageOfButton & usage)
{
	switch (usage)
	{
	case usageOfButton::BUYWEAPON1:
		handleBuyingWeapon(0);
		break;
	case usageOfButton::BUYWEAPON2:
		handleBuyingWeapon(1);
		break;
	case usageOfButton::BUYWEAPON3:
		handleBuyingWeapon(2);
		break;


	// *_1 - amunicja do broni energetycznej
	case usageOfButton::BUYAMMOPACK1_1:
		this->handleBuyingAmmunition(typeOfWeapon::ENERGETIC, this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC]);
		break;
	case usageOfButton::BUYAMMOPACK1_2:
		this->handleBuyingAmmunition(typeOfWeapon::ENERGETIC, 5*this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC]);
		break;
	case usageOfButton::BUYAMMOPACK1_3:
		this->handleBuyingAmmunition(typeOfWeapon::ENERGETIC, 10* this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC]);
		break;

	// *_2 - amunicja do broni o³owiowej
	case usageOfButton::BUYAMMOPACK2_1:
		this->handleBuyingAmmunition(typeOfWeapon::BULLETTYPE, this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]);
		break;
	case usageOfButton::BUYAMMOPACK2_2:
		this->handleBuyingAmmunition(typeOfWeapon::BULLETTYPE, 5*this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]);
		break;
	case usageOfButton::BUYAMMOPACK2_3:
		this->handleBuyingAmmunition(typeOfWeapon::BULLETTYPE, 10*this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]);
		break;

	// *_3 - amunicja do broni plazmowej
	case usageOfButton::BUYAMMOPACK3_1:
		this->handleBuyingAmmunition(typeOfWeapon::PLASMA, this->HowManyBltsInPacks[typeOfWeapon::PLASMA]);
		break;
	case usageOfButton::BUYAMMOPACK3_2:
		this->handleBuyingAmmunition(typeOfWeapon::PLASMA, 5* this->HowManyBltsInPacks[typeOfWeapon::PLASMA]);
		break;
	case usageOfButton::BUYAMMOPACK3_3:
		this->handleBuyingAmmunition(typeOfWeapon::PLASMA, 10 * this->HowManyBltsInPacks[typeOfWeapon::PLASMA]);
		break;

	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::switchState(statesOfShip switchToWhat)
{
	// co by siê nie dzia³o to dla ka¿dej zmiany stanów musimy usun¹æ aktualne guziczki
	buttons.clear();

	switch (switchToWhat)
	{
	case statesOfShip::SHOP:
	{
		this->initializeShop();
		this->inWhichRoomWeAre = statesOfShip::SHOP;
		break;
	}
	case statesOfShip::VIRTUALARENA:
	{
		this->initializeArena();
		this->inWhichRoomWeAre = statesOfShip::VIRTUALARENA;
		break;
	}
	case statesOfShip::IMPLANTATIONSTATION:
	{
		this->initializeStation();
		this->inWhichRoomWeAre = statesOfShip::IMPLANTATIONSTATION;
		break;
	}
	case statesOfShip::FIGHT:
	{
		buttons.clear();
		SettingUpWaveTimeElapsed.restart();
		this->currentState = statesOfPlayGameState::SETTINGUPWAVE;
		break;
	}
	case statesOfShip::INTERGALAXYPORT:
	{
		this->intializePort();
		this->inWhichRoomWeAre = statesOfShip::INTERGALAXYPORT;
		break;
	}
	case statesOfShip::MAINMENU:
	{
		this->inWhichRoomWeAre = statesOfShip::MAINMENU;
		this->initializeMainMenuShipState();
		break;
	}
	}

	if (switchToWhat != statesOfShip::MAINMENU && switchToWhat != statesOfShip::FIGHT)
	{
		// guzik powrotu do menu g³ównego
		button = new Button(usageOfButton::goToMainMenu, sf::Vector2f(getWx(Window) / 4, getWy(Window) / 8),
			sf::Vector2f(getWx(Window) - getWx(Window) / 4 - getWx(Window) / 32, getWy(Window) - getWy(Window) / 8 - getWy(Window) / 32), true);
		button->setText("Powrót!", this->Font);
		buttons.push_back(*button);
		delete button;
		button = NULL;
	}

	// ma³y delay podczas zmiany
	Sleep(100);

}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::generateLineOfButtons(int howMany, int addToIter, sf::Vector2f size, sf::Vector2f StartingPos, sf::Color passiveColor, sf::Color interactedColor, bool generateInteractable)
{
	// g³ówne guziczki do zakupu broni
	for (int i = 0; i < howMany; ++i)
	{
		button = new Button(static_cast<usageOfButton>(i + addToIter), sf::Vector2f(size.x*getWx(Window), size.y*getWx(Window)),
			sf::Vector2f(StartingPos.x + (i * size.x*getWx(Window)) + i *SPACER*getWx(Window), StartingPos.y), generateInteractable);

		button->setColors(passiveColor, interactedColor);
		buttons.push_back(*button);
		delete button;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::initializeShop()
{
	double size = (1.0 - 4 * SPACER) / 3;

	// g³ówne guziczki do zakupu broni
	generateLineOfButtons(3, 7, sf::Vector2f(size, size), sf::Vector2f(SPACER*getWx(Window), SPACER*getWx(Window)), sf::Color::Color(0x64, 0x80, 0x10), sf::Color::Color(0xFF, 0x0, 0x0), true);

	// guziczki do zakupu amunicji ( pakiet 1)
	for (int i = 0; i < 3; ++i)
	{
		generateLineOfButtons(3, 10 + i * 3, sf::Vector2f(size, size / 5), sf::Vector2f(SPACER*getWx(Window), buttons[0].getGlobalBounds().height + (i + 1)*1.5*SPACER*getWx(Window) + i*getWy(Window) / 32 + getWy(Window) /12),
			sf::Color::Color(0x64, 0x80, 0x10), sf::Color::Color(0xFF, 0x0, 0x0),true);
	}
	/*******************************************************************************/
	// zabawa tekstami
	buttons[3].setText("E: "+std::to_string(this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC])+" ("+std::to_string(HowManyBltsInPacks[typeOfWeapon::ENERGETIC] * costsOfBullets[typeOfWeapon::ENERGETIC])+")", this->Font);
	buttons[6].setText("E: "+std::to_string(5*this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC])+" ("+std::to_string(5*HowManyBltsInPacks[typeOfWeapon::ENERGETIC] * costsOfBullets[typeOfWeapon::ENERGETIC])+")", this->Font);
	buttons[9].setText("E: " + std::to_string(10 * this->HowManyBltsInPacks[typeOfWeapon::ENERGETIC]) + " (" + std::to_string(10 * HowManyBltsInPacks[typeOfWeapon::ENERGETIC] * costsOfBullets[typeOfWeapon::ENERGETIC]) + ")", this->Font);

	buttons[4].setText("O: "+std::to_string(this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]) + " (" + std::to_string(HowManyBltsInPacks[typeOfWeapon::BULLETTYPE] * costsOfBullets[typeOfWeapon::BULLETTYPE]) + ")", this->Font);
	buttons[7].setText("O: " + std::to_string(5*this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]) + " (" + std::to_string(5*HowManyBltsInPacks[typeOfWeapon::BULLETTYPE] * costsOfBullets[typeOfWeapon::BULLETTYPE]) + ")", this->Font);
	buttons[10].setText("O: " + std::to_string(10*this->HowManyBltsInPacks[typeOfWeapon::BULLETTYPE]) + " (" + std::to_string(10*HowManyBltsInPacks[typeOfWeapon::BULLETTYPE] * costsOfBullets[typeOfWeapon::BULLETTYPE]) + ")", this->Font);

	buttons[5].setText("P: " + std::to_string(this->HowManyBltsInPacks[typeOfWeapon::PLASMA]) + " (" + std::to_string(HowManyBltsInPacks[typeOfWeapon::PLASMA] * costsOfBullets[typeOfWeapon::PLASMA]) + ")", this->Font);
	buttons[8].setText("P: " + std::to_string(5*this->HowManyBltsInPacks[typeOfWeapon::PLASMA]) + " (" + std::to_string(5*HowManyBltsInPacks[typeOfWeapon::PLASMA] * costsOfBullets[typeOfWeapon::PLASMA]) + ")", this->Font);
	buttons[11].setText("P: " +std::to_string(10*this->HowManyBltsInPacks[typeOfWeapon::PLASMA]) + " (" + std::to_string(10 *HowManyBltsInPacks[typeOfWeapon::PLASMA] * costsOfBullets[typeOfWeapon::PLASMA]) + ")", this->Font);
	/*******************************************************************************/


	/*******************************************************************************/
	// zabawa kolorkami
	for (int i = 3; i <= 9; i += 3)
	{
		buttons[i].setColors(colorMap[typeOfWeapon::ENERGETIC], sf::Color::Color(0xff,0x0,0x0));
		buttons[i].setFillColor(sf::Color::Color(0xDA, 0xA5, 0x20));
	}
	for (int i = 4; i <= 10; i += 3)
	{
		buttons[i].setColors(colorMap[typeOfWeapon::BULLETTYPE], sf::Color::Color(0xff, 0x0, 0x0));
		buttons[i].setFillColor(sf::Color::Color(0xDA, 0xA5, 0x20));
	}
	for (int i = 5; i <= 11; i += 3)
	{
		buttons[i].setColors(colorMap[typeOfWeapon::PLASMA], sf::Color::Color(0xff, 0x0, 0x0));
		buttons[i].setFillColor(sf::Color::Color(0xDA, 0xA5, 0x20));
	}
	/*******************************************************************************/

	if (actualAssortiment.size() > 0)
		buttons[0].setText(std::to_string(actualAssortiment[0].ID), Font);

	if (actualAssortiment.size() > 1)
		buttons[1].setText(std::to_string(actualAssortiment[1].ID), Font);

	if (actualAssortiment.size() > 2)
		buttons[2].setText(std::to_string(actualAssortiment[2].ID), Font);


}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::intializePort()
{
	double size = (1.0 - 8 * SPACER) / 3;
	generateLineOfButtons(3, 19, sf::Vector2f(size, 2 * size), sf::Vector2f(3 * SPACER * getWx(Window), 2 * SPACER * getWy(Window)), sf::Color::Red, sf::Color::Color(0x0, 0xff, 0x0),true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interpratePortSignals(usageOfButton & usage)
{
	switch (usage)
	{
	case usageOfButton::TAKEQUEST_1:
		MessageBox(NULL, "Wybieram Zadanie 1", NULL, NULL);
		break;
	case usageOfButton::TAKEQUEST_2:
		MessageBox(NULL, "Wybieram Zadanie 2", NULL, NULL);
		break;
	case usageOfButton::TAKEQUEST_3:
		MessageBox(NULL, "Wybieram Zadanie 3", NULL, NULL);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::initializeArena()
{
	double size = (1.0 - 8 * SPACER) / 3;
	generateLineOfButtons(3, 22, sf::Vector2f(size, 2 * size), sf::Vector2f(3 * SPACER * getWx(Window), 2 * SPACER * getWy(Window)), sf::Color::Red, sf::Color::Color(0x0, 0xff, 0x0),true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateArenaSignals(usageOfButton & usage)
{
	switch (usage)
	{
	case usageOfButton::FIGHTBOSS_1:
		MessageBox(NULL, "Wybrano Walkê z bossem po lewej \n", NULL, NULL);
		break;
	case usageOfButton::FIGHTBOSS_2:
		MessageBox(NULL, "Wybrano Walkê z bossem po œrodku \n", NULL, NULL);
		break;
	case usageOfButton::FIGHTBOSS_3:
		MessageBox(NULL, "Wybrano Walkê z bossem po prawej \n", NULL, NULL);
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::intializeCabine()
{
	// (!)
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateCabineSignals(usageOfButton & usage)
{
	// (!)
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::initializeStation()
{
	double size = (1.0 - 8 * SPACER) / 4;
	generateLineOfButtons(4, 25, sf::Vector2f(size, size), sf::Vector2f(2.5*SPACER * getWx(Window), SPACER * getWy(Window)), sf::Color::Red, sf::Color::Color(0x0, 0xff, 0x0),true);
	generateLineOfButtons(4, 29, sf::Vector2f(size, size), sf::Vector2f(2.5*SPACER * getWx(Window), size*getWy(Window) + 4 * SPACER * getWy(Window)), sf::Color::Red, sf::Color::Color(0x0, 0xff, 0x0),true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::interprateStationSignals(usageOfButton & usage)
{
	switch (usage)
	{
	case usageOfButton::CHOOSEDIMPLANT_1:
		MessageBox(NULL, "Wybrano implant 1", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_2:
		MessageBox(NULL, "Wybrano implant 2", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_3:
		MessageBox(NULL, "Wybrano implant 3", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_4:
		MessageBox(NULL, "Wybrano implant 4", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_5:
		MessageBox(NULL, "Wybrano implant 5", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_6:
		MessageBox(NULL, "Wybrano implant 6", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_7:
		MessageBox(NULL, "Wybrano implant 7", NULL, NULL);
		break;

	case usageOfButton::CHOOSEDIMPLANT_8:
		MessageBox(NULL, "Wybrano implant 8", NULL, NULL);
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////