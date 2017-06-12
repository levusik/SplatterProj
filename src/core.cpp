#include "playGameState.h"
#define m_CurrSelectedWpn (this->backpack[this->indexOfCurrentlySelectedWpn])
#define cast(type,x)	(static_cast<type>(x))

/////////////////////////////////////////////////////////////////////////////////////////////
playGameState::playGameState(sf::Font & font, sf::RenderWindow &window) :
	Player(32.f, sf::Color::Color(0x00, 0x0, 0xFF), sf::Vector2f(WINDOWWIDTH / 2, WINDOWHEIGHT / 2)),
	Window(window), Font(font), howManyButtons(6)
{

	this->StaticView = Window.getView();

	srand(time(NULL));
	// liczby do kontrolowania kamery
	xViewOffset = yViewOffset = SpawnPointIndex = allChance = 0;
	isPlayingAnimation = false;

	// ustawienie textu
	text.setFont(font);
	counter.setFont(font);
	counter.setCharacterSize(256.f);

	debugFile.open("debug.txt", std::ios::out | std::ios::trunc);

	// ustawiamy stan pocz�tkowy 
	this->currentState = statesOfPlayGameState::SHIP;

	this->currency			= 0;
	this->previousMagSize	= 0;
	this->prevWpnType		= typeOfWeapon::_NULL;


	// (!) 
	// zrobi� odczyt z pliku
	this->colorMap[typeOfWeapon::BULLETTYPE]	= sf::Color::Color(0x88,0x88,0x88);
	this->colorMap[typeOfWeapon::ENERGETIC]     = sf::Color::Color(0x0,0x9a,0xCD);
	this->colorMap[typeOfWeapon::PLASMA]	    = sf::Color::Color(0x0,0xff,0x7f);

}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::run()
{
	initializeState();
	while (true)
	{

		switch (currentState)
		{
		case statesOfPlayGameState::SETTINGUPWAVE:
		{
			handleSettingUpWave();
			break;
		}
		case statesOfPlayGameState::PLAYINGWAVE:
		{
			updatePlayingWaveState();
			displayPlayingWaveState();
			break;
		}
		case statesOfPlayGameState::SHIP:
		{
			handleShipState();
			break;
		}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::loadTemplatesAndItemsFromFile()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//	Wczytanie jednostek z pliku 
	std::fstream splatUnitsDatabase("splatDatabase.txt", std::ios::in | std::ios::out);
	std::fstream weaponDatabase("weaponDatabase.txt", std::ios::in || std::ios::out);
	std::string  line;
	std::string  refactorizedString;
	std::vector<std::string> values;

	while (std::getline(splatUnitsDatabase, line))
	{
		// usuwamy komentarze
		line = filtrateString(line);
		std::string phrase;

		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				if (phrase.size())
				{
					values.push_back(phrase);
				}
				phrase.clear();
			}
			else if (line[i] != '\n' && line[i] != '\t')
			{
				phrase += line[i];
			}

		}
		if (phrase.size())
			values.push_back(phrase);

		// je�eli wektor osi�gnie pewn� ilo�� warto�ci to 
		// wype�niamy nimi szablon Splata
		if (values.size() == numOfValLoadedFromFile)
		{
			// bezsensownie napisane linijki kodu ...
			splatUnit.changeMoveBy = atoi(values[0].c_str());
			splatUnit.maxArmor = atoi(values[1].c_str());
			splatUnit.maxDamage = atoi(values[2].c_str());
			splatUnit.maxHP = atoi(values[3].c_str());
			splatUnit.minArmor = atoi(values[4].c_str());
			splatUnit.minDamage = atoi(values[5].c_str());
			splatUnit.minHP = atoi(values[6].c_str());
			splatUnit.rangeOfWeapon = atoi(values[7].c_str());
			splatUnit.minSpeed = atoi(values[8].c_str());
			splatUnit.maxSpeed = atoi(values[9].c_str());
			splatUnit.acceleration = atoi(values[10].c_str());
			splatUnit.sizeOfView = atoi(values[11].c_str());
			splatUnit.pith = atoi(values[12].c_str()) / 10000.f;
			splatUnit.agressiveModeColor = sf::Color::Color(atoi(values[13].c_str()), atoi(values[14].c_str()), atoi(values[15].c_str()));
			splatUnit.passiveModeColor = sf::Color::Color(atoi(values[16].c_str()), atoi(values[17].c_str()), atoi(values[18].c_str()));
			splatUnit.reactingtoPlayerColor = sf::Color::Color(atoi(values[19].c_str()), atoi(values[20].c_str()), atoi(values[21].c_str()));
			splatUnit.ChargingColor = sf::Color::Color(atoi(values[22].c_str()), atoi(values[23].c_str()), atoi(values[24].c_str()));
			splatUnit.fleeingColor = sf::Color::Color(atoi(values[25].c_str()), atoi(values[26].c_str()), atoi(values[27].c_str()));
			splatUnit.attackingPlayerColor = sf::Color::Color(atoi(values[28].c_str()), atoi(values[29].c_str()), atoi(values[30].c_str()));
			splatUnit.timeToMakeDecisionInSec = atoi(values[31].c_str()) / 1000.f;
			splatUnit.chargeDistance = atoi(values[32].c_str()) + Player.getCircle().getRadius();
			splatUnit.radius = atoi(values[33].c_str());
			splatUnit.safeDistance = atoi(values[34].c_str()) + Player.getCircle().getRadius() + splatUnit.radius;
			splatUnit.chanceOfGettingRandomized = atoi(values[35].c_str());
			this->allChance += splatUnit.chanceOfGettingRandomized;

			splatTemplateDatabase.push_back(splatUnit);

			if (DEBUG)
			{
				for (int i = 0; i < values.size(); ++i)
				{
					debugFile << atoi(values[i].c_str()) << "\n";
				}
			}
			values.clear();

		}
	}
	if (!DEBUG)
	{
		debugFile << splatTemplateDatabase.size() << "\n";
		debugFile << "********************************************************************************\n";
		for (int i = 0; i < splatTemplateDatabase.size(); ++i)
		{
			char buff[10];
			debugFile << "Change Move By:" << splatTemplateDatabase[i].changeMoveBy << "\n";
			debugFile << "maks. pancerz :" << splatTemplateDatabase[i].maxArmor << "\n";
			debugFile << "maks. damage :" << splatTemplateDatabase[i].maxDamage << "\n";
			debugFile << "maks. HP :" << splatTemplateDatabase[i].maxHP << "\n";
			debugFile << "min. pancerz : " << splatTemplateDatabase[i].minArmor << "\n";
			debugFile << "min. damage : " << splatTemplateDatabase[i].minDamage << "\n";
			debugFile << "min. HP:" << splatTemplateDatabase[i].minHP << "\n";
			debugFile << "zasi�g broni :" << splatTemplateDatabase[i].rangeOfWeapon << "\n";
			debugFile << "min. speed :" << splatTemplateDatabase[i].minSpeed << "\n";
			debugFile << "maks. speed: " << splatTemplateDatabase[i].maxSpeed << "\n";
			debugFile << "przyspieszenie : " << splatTemplateDatabase[i].acceleration << "\n";
			debugFile << "rozmiar widoku : " << splatTemplateDatabase[i].sizeOfView << "\n";
			debugFile << "waga algorytmu: " << splatTemplateDatabase[i].pith << "\n";
			_itoa_s(splatTemplateDatabase[i].agressiveModeColor.toInteger(), buff, 16);
			debugFile << "kolor gdy agresywny : " << buff << "\n";
			_itoa_s(splatTemplateDatabase[i].passiveModeColor.toInteger(), buff, 16);
			debugFile << "kolor gdy pasywny : " << buff << "\n";
			_itoa_s(splatTemplateDatabase[i].reactingtoPlayerColor.toInteger(), buff, 16);
			debugFile << "kolor gdy reaguje na gracza :" << buff << "\n";
			_itoa_s(splatTemplateDatabase[i].ChargingColor.toInteger(), buff, 16);
			debugFile << "kolor gdy szar�uje : " << buff << "\n";
			_itoa_s(splatTemplateDatabase[i].fleeingColor.toInteger(), buff, 16);
			debugFile << "kolor gdy ucieka :" << buff << "\n";
			_itoa_s(splatTemplateDatabase[i].attackingPlayerColor.toInteger(), buff, 16);
			debugFile << "kolor gdy atakuje : " << buff << "\n";
			debugFile << "czas po kt�rym podejmuje decyzj� : " << splatTemplateDatabase[i].timeToMakeDecisionInSec << "\n";
			debugFile << "dystans po kt�rym szar�uje : " << splatTemplateDatabase[i].chargeDistance << "\n";
			debugFile << "promie�: " << splatTemplateDatabase[i].radius << "\n";
			debugFile << "bezpieczny dystans : " << splatTemplateDatabase[i].safeDistance << "\n";
			debugFile << "szansa na zostanie wyloswanym " << splatTemplateDatabase[i].chanceOfGettingRandomized << "\n";
			debugFile << "********************************************************************************\n";
		}
	}


	values.clear();


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// wczytanie broni z plik�w

	while (std::getline(weaponDatabase, line))
	{
		std::string phrase;
		line = filtrateString(line);


		for (int i = 0; i < line.size(); ++i)
		{
			if (line[i] == ' ')
			{
				if (phrase.size())
				{
					values.push_back(phrase);
				}
				phrase.clear();
			}
			else if (line[i] != '\n' && line[i] != '\t')
			{
				phrase += line[i];
			}

		}
		if (phrase.size())
			values.push_back(phrase);

		if (values.size() == 7)
		{
			rangeWeapon weapon;
			weapon.minDamage = std::stoi(values[0]);
			weapon.maxDamage = std::stoi(values[1]);
			weapon.bulletSize = std::stoi(values[2]);
			weapon.firerate = std::stoi(values[3]);
			weapon.recoil = std::stoi(values[4]);
			weapon.minSpeedOfBullet = std::stoi(values[5]);
			weapon.maxSpeedOfBullet = std::stoi(values[6]);

			effects Effects;
			weapon.setEffects(Effects);

			rangeWeaponDatabase.push_back(weapon);
			values.clear();
		}
	}

	if (DEBUG)
	{
		for (int i = 0; i < values.size(); ++i)
		{
			debugFile << values[i] << "\n";
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// zarz�dzanie amunicj�
	this->Ammunitions[typeOfWeapon::BULLETTYPE] = 100;
	this->Ammunitions[typeOfWeapon::ENERGETIC] = 200;
	this->Ammunitions[typeOfWeapon::PLASMA] = 50;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::initializeState()
{
	DynamicView = this->Window.getView();

	// wczytanie szablon�w jednostek i broni 
	loadTemplatesAndItemsFromFile();

	// (!)
	// pocz�tkowymi bro�mi gracza b�dzie podstawowy karabin energetyczny, plazmowy i o�owiowy
	this->backpack.push_back(rangeWeaponDatabase[0]);
	this->indexOfCurrentlySelectedWpn = 0;

	rangeWeapon wpn = rangeWeaponDatabase[0];
	wpn.EffectWeaponCauses.type = typeOfWeapon::ENERGETIC;
	backpack.push_back(wpn);

	wpn.EffectWeaponCauses.type = typeOfWeapon::PLASMA;
	backpack.push_back(wpn);




	// (!)
	// TODO: sensowniejsze rozplanowanie tekst�w
	text.setCharacterSize(70);
	text.setFillColor(sf::Color::Color(164, 164, 0));
	text.setPosition(this->xViewOffset, 0);
	infoTexts.push_back(text);

	text.setFillColor(sf::Color::Color(128, 128, 0));
	text.setPosition(this->xViewOffset + 150, 0);
	infoTexts.push_back(text);


	text.setFillColor(sf::Color::Color(0x0,0xff,0x0));
	text.setPosition(sf::Vector2f(WINDOWWIDTH - 300,0));
	ammoAndCurrencyTexts.push_back(text);

	text.setFillColor(sf::Color::Color(0x0, 0xA0, 0x0));
	text.setPosition(sf::Vector2f(WINDOWWIDTH - 450,0));
	ammoAndCurrencyTexts.push_back(text);

	text.setFillColor(sf::Color::Color(0xff,0xb9,0x0f));
	text.setPosition(sf::Vector2f(0,100));
	ammoAndCurrencyTexts.push_back(text);

}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::displayPlayingWaveState()
{
	this->Window.clear();

	this->Window.draw(Player.getCircle());
	this->Window.draw(Player);
	this->Window.draw(Player.getLine());

	// rysowanie wszystkich splat�w
	for (int i = 0; i < splatArray.size(); ++i)
	{
		for (int j = 0; j < splatArray[i].size(); ++j)
		{
			this->Window.draw(splatArray[i][j].getCircle());
			if (DEBUG)
				this->Window.draw(splatArray[i][j].getVertex());
		}
	}

	// rysowanie pocisk�w
	for (int i = 0; i < ProjectilesArray.size(); ++i)
		this->Window.draw(ProjectilesArray[i]);

	// rysowanie text�w
	for (int i = 0; i < infoTexts.size(); ++i)
		this->Window.draw(infoTexts[i]);

	//rysowanie tekst�w z informacj� o amunicji
	for (int i = 0; i < ammoAndCurrencyTexts.size(); ++i)
		this->Window.draw(ammoAndCurrencyTexts[i]);

	this->Window.display();


}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::updatePlayingWaveState()
{
	sf::Event event;
	while (this->Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			exit(0);
		}
	}

	// zarz�dzanie widokiem
	manageView();

	Player.update(sf::Vector2i(sf::Mouse::getPosition(this->Window).x + xViewOffset, sf::Mouse::getPosition(this->Window).y + yViewOffset));

	// zarz�dzanie usuwaniem pocisk�w
	handleErasingProjectiles();

	// zarz�dzanie strzelaniem
	handleShooting();

	// wywo�anie metody kt�ra zajmie si� zarz�dzaniem przeciwnik�w
	handleEnemies();

	// obs�uga wykrywania kolizji
	handleCollisionDetection();

	// zarz�dzanie GUI
	updateGUI();

	// ko�czymy stan gdy pozosta�o 0 przeciwnik�w 
	if (this->howManyCreaturesLeft == 0)
	{
		changeToCity();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::manageView()
{
	// Wyja�nienie :
	// dajemy naszej postaci prostokat w kt�rym mo�e si� porusza� bez zmiany widoku 
	// (wymiary tego prostok�ta to (WINDOWWIDTH/4, WIINDOWHEIGHT/4,WINDOWIDTH/2,WINDOWHEIGHT/2)	)
	// je�eli pozycja b�dzie w kt�rym� z "pask�w" to przesuwamy tak aby zawsze gracz znajdowa� si� w 
	// wyznaczonym prostok�cie
	// 
	//	********
	//	**####**
	//	**####**
	//	********	
	//
	// * - zmiana widoku
	// # - mo�na si� porusza�
	int moveVx, moveVy;
	moveVx = moveVy = 0;

	if (Player.getPosition().x < WINDOWWIDTH / 4 + xViewOffset)
	{
		moveVx = -Player.getVx();
	}
	if (Player.getPosition().x > 3 * WINDOWWIDTH / 4 + xViewOffset)
	{
		moveVx = Player.getVx();
	}
	if (Player.getPosition().y < WINDOWHEIGHT / 4 + yViewOffset)
	{
		moveVy = -Player.getVy();

	}
	if (Player.getPosition().y > 3 * WINDOWHEIGHT / 4 + yViewOffset)
	{
		moveVy = Player.getVy();
	}
	xViewOffset += moveVx;
	yViewOffset += moveVy;
	DynamicView.move(moveVx, moveVy);
	moveImportantTexts(moveVx, moveVy);

	this->Window.setView(DynamicView);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleShooting()
{
	// update'ujemy teksty z amunicj�
	this->manageAmmoAndCurrencyTexts();

	// rysujemy teksty z amunicj�
	for (int i = 0; i < ammoAndCurrencyTexts.size(); ++i)
		this->Window.draw(ammoAndCurrencyTexts[i]);

	// zarz�dzamy plecakiem
	this->manageBackpack();

	// update paremtr�w gracza kt�re chcemy przekaza� do gracza

	this->PlayerParameters.position.x = this->Player.getCenter().x;
	this->PlayerParameters.position.y = this->Player.getCenter().y;

	this->PlayerParameters.angle = atan2(sf::Mouse::getPosition(Window).y + yViewOffset - this->Player.getCenter().y, sf::Mouse::getPosition(Window).x + xViewOffset - this->Player.getCenter().x);

	// wywo�ujemy metod� broni kt�ra zajmie si� strzelaniem
	m_CurrSelectedWpn.takeAction(this->Ammunitions, this->ProjectilesArray, PlayerParameters);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleEnemies()
{
	// obliczamy wszystkie potrzebne warto�ci do ruchu grupy 
	// (a dok�adniej obliczamy �redni� pr�dko�� oraz �redni� odleg�o�� od boid�w)
	calculateBoidRules();



	// update naszych wszystkich orkas�w
	for (int i = 0; i < splatArray.size(); ++i)
	{
		for (int j = 0; j < splatArray[i].size(); ++j)
		{
			splatArray[i][j].setParametersOfOtherMates(positionsOfOtherBoids[i][j]);
			splatArray[i][j].update(averageV[i], averageDistance[i], Verticle{ Player.getCircle().getPosition().x,Player.getCircle().getPosition().y });
		}
	}


}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::updateGUI()
{
	// najpierw zajmujemy si� spraw� infoTexts

	// obliczamy ile stwork�w jeszcze zosta�o
	int howManySplatsLeft = 0;
	for (int i = 0; i < splatArray.size(); ++i)
		howManySplatsLeft += splatArray[i].size();

	this->howManyCreaturesLeft = howManySplatsLeft;
	infoTexts[0].setString(std::to_string(howManySplatsLeft));


	// update'ujemy 
	infoTexts[1].setString(std::to_string(static_cast<int>(this->timeElapsedWave.getElapsedTime().asSeconds())));

}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::generateSpawnPoint(int howManySpawnPoints)
{
	//////////////////////////////////////////////////////////////////////////
	//			RUSZTOWANIE
	// tutaj wybierzemy losow� pozycje ale na razie (w ramach test�w) 
	// wybierzemy sobie ustalon� pozycj�
	sf::Vector2f spawnPoint;

	spawnPoint = sf::Vector2f(120, 120);


	splat Splat;

	// aktualna grupa do kt�rej b�dziemy dodawa� przeciwnik�w.
	// po dodaniu bajer�w dodamy vectora do vectora vector�w ( dzi�ki temu otrzymamy od razu podzia� na specyfinczne grupy
	// co u�atwi nam potem robot�)
	std::vector<splat> currentGroup;

	for (int i = 0; i < howManySpawnPoints; ++i)
	{
		// zak�adamy �e ka�da grupa sk�ada si� z jednakowych splat�w
		int randomIndex = rand() % splatTemplateDatabase.size();
		splatTemplate sTemplate = getRandomSplatTemplate();
		debugFile << randomIndex << "\n";

		// (!)
		//int howManyEnemies = rand()% 20 + 10;
		int howManyEnemies = 1;

		for (int j = 0; j < howManyEnemies; ++j)
		{
			Splat.create(i, sTemplate);
			currentGroup.push_back(Splat);
		}
		splatArray.push_back(currentGroup);
	}



	//////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::calculateBoidRules()
{
	// czy�cimy wszystkie wektory 
	averageDistance.clear();
	averageV.clear();
	positionsOfOtherBoids.clear();

	// link : http://www.algorytm.org/sztuczna-inteligencja/boidy.html

	// wyodr�bniamy konkretn� grup� splat�w
	std::vector<splat> currentGroup;

	// zmienne na kt�rych b�dziemy przeprowadza� operacje
	Verticle avgV;
	double avgDistance;
	int	   avgCounter, avgDistanceCounter;


	// wektory kt�re troch� nam u�atwi� rozumowanie i my�lenie 
	std::vector<std::vector<otherMatesParameters>>  ParametersOfOneGroup;
	std::vector<otherMatesParameters>		   	    ParametersOfOneCreature;
	otherMatesParameters						    ParametersOfCreature;

	// iterujemy dla wszystkich grup przeciwnik�w
	for (int i = 0; i < splatArray.size(); ++i)
	{
		// zerujemy nasze �rednie  
		avgV.blank();
		avgDistance = 0;
		avgCounter = avgDistanceCounter = 0;
		// iterujemy dla ka�dego orka w grupie 
		for (int j = 0; j < splatArray[i].size(); ++j)
		{



			// regu�a pierwsza : obliczamy �redni� pr�dko�� ka�dego boida z danej grupy 
			avgV.x += splatArray[i][j].getV().x;
			avgV.y += splatArray[i][j].getV().y;
			avgCounter++;

			// regu�a druga :
			// iterujemy dla ka�dego przeciwnika w grupie 
			//
			for (int k = 0; k < splatArray[i].size(); ++k)
			{

				// obliczamy �redni dystans
				avgDistance += calculateDistance(splatArray[i][j].getCenter(), splatArray[i][k].getCenter());
				avgDistanceCounter++;


				// mo�na to zmie�ci� w jednej linijce ale lepiej to podpi�� pod syntatic sugar 
				ParametersOfCreature.position.x = splatArray[i][k].getCenter().x;
				ParametersOfCreature.position.y = splatArray[i][k].getCenter().y;
				ParametersOfCreature.hasDetectedPlayer = splatArray[i][k].hasCreatureDetectedPlayer();

				ParametersOfOneCreature.push_back(ParametersOfCreature);
			}
			ParametersOfOneGroup.push_back(ParametersOfOneCreature);
			ParametersOfOneCreature.clear();
		}




		positionsOfOtherBoids.push_back(ParametersOfOneGroup);
		ParametersOfOneGroup.clear();
		ParametersOfOneCreature.clear();

		avgV.x /= avgCounter;
		avgV.y /= avgCounter;
		avgDistance /= avgDistanceCounter;

		averageV.push_back(avgV);
		averageDistance.push_back(avgDistance);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
double playGameState::calculateDistance(Verticle position1, Verticle position2)
{
	double val = sqrt(pow(position1.x - position2.x, 2) + pow(position1.y - position2.y, 2));
	if (val != 0)
	{
		return val;
	}
	else
		return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleCollisionDetection()
{
	int											  projCounter, splatGroupCounter, splatCounter;
	std::vector<splat>::iterator				  splatIter;
	std::vector<projectile>::iterator			  projIter;
	std::vector<std::vector<splat>>::iterator	  splatGroupIter;

	// iterujemy przez wszystkie pociski i obliczamy sobie dystans do ka�dego z splat�w
	// niestety, nie dokszta�ci�em si� wykszta�caj�co z collision detection przez co mamy 
	// n^2 g�upoty :/
	// (!) Refaktoryzacja 

	splatGroupCounter = splatArray.size() - 1;
	// iterujemy przez wszystkie grupy
	for (splatGroupIter = splatArray.end() - 1, splatGroupCounter = splatArray.size() - 1; splatGroupIter != splatArray.begin() - 1; --splatGroupIter, --splatGroupCounter)
	{
		// iterujemy przez ka�dego splata
		for (splatIter = splatArray[splatGroupCounter].end() - 1, splatCounter = splatArray[splatGroupCounter].size() - 1; splatIter != splatArray[splatGroupCounter].begin() - 1; --splatIter, --splatCounter)
		{
			// sprawdzamy czy przypadkiem tak si� nie sta�o �e dany splat nie koliduje z pociskiem
			// a sprawdzamy to przez policzenie sobie elegancko dystans�w od �rodka pocisku do �rodka splata
			for (projIter = ProjectilesArray.end() - 1, projCounter = ProjectilesArray.size() - 1; projIter != ProjectilesArray.begin() - 1; --projIter, --projCounter)
			{
				if (calculateDistance(ProjectilesArray[projCounter].getCenter(), splatArray[splatGroupCounter][splatCounter].getCenter()) <=
					ProjectilesArray[projCounter].getRadius() + splatArray[splatGroupCounter][splatCounter].getCircle().getRadius())
				{
					// je�eli wykryto kolizj� to zadajemy damage splatowi
					ProjectilesArray.erase(projIter);
					bulletParameters params;
					params = ProjectilesArray[projCounter];
					splatArray[splatGroupCounter][splatCounter].dealDamage(params);

					// je�eli splatowi zadano damage to alarmujemy splata kt�ry dosta� oraz 
					// jego najbli�szego (przynajmniej w kontek�cie informatycznym ) kumpla
					splatArray[splatGroupCounter][splatCounter].alertEnemy();

					if (splatCounter > 0)
						splatArray[splatGroupCounter][splatCounter - 1].alertEnemy();
					else
						splatArray[splatGroupCounter][splatCounter + 1].alertEnemy();
				}
			}
			// je�eli splat zosta� zabity to usuwamy go z wektora oraz dodajemy jego
			// warto�� do naszej waluty
			if (!splatArray[splatGroupCounter][splatCounter].isCreatureAlive())
			{
				this->currency += splatArray[splatGroupCounter][splatCounter].getWorth();
				splatArray[splatGroupCounter].erase(splatIter);
			}

		}
	}


}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::moveImportantTexts(int vecX, int vecY)
{
	// poruszamy infoTexts 
	for (int i = 0; i < infoTexts.size(); ++i)
			infoTexts[i].move(vecX, vecY);

	// poruszamy tekstami wy�wietlaj�cymi amunicj� i pieni�dze 
	for (int i = 0; i < ammoAndCurrencyTexts.size(); ++i)
		ammoAndCurrencyTexts[i].move(vecX, vecY);


	if (this->isPlayingAnimation)
	{
		this->victoryText.move(vecX, vecY);
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleErasingProjectiles()
{
	if (ProjectilesArray.size())
	{
		std::vector<projectile>::iterator iter;
		int counter;
		// update pocisk�w oraz usuni�cie wszystkich zb�dnych pocisk�w
		for (iter = ProjectilesArray.end() - 1, counter = ProjectilesArray.size() - 1; iter != ProjectilesArray.begin() - 1; --iter, --counter)
		{
			ProjectilesArray[counter].update();

			// je�eli zegar b�dzie wi�kszy ni� maksymalny czas Projekcji 
			if (ProjectilesArray[counter].getTime().asSeconds() > ProjectilesArray[counter].getMaxTime())
			{
				ProjectilesArray.erase(iter);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::roundValue(int size, int & value)
{
	// metoda kt�ra zrobi nam normalne modulo a nie C++'owe modulo 
	// a.k.a -1 % 3 b�dzie dawa� 2 a nie -1 
	if (value < 0)
	{
		value = size - 1;
	}
	else
	{
		value %= size;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::manageBackpack()
{
	if (delayBtwChangingWeapon.getElapsedTime().asSeconds() > 0.4f)
	{
		// je�eli wci�ni�to N to bierzemy nast�pn� bro�
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
		{
			this->indexOfCurrentlySelectedWpn += 1;
			roundValue(this->backpack.size(), this->indexOfCurrentlySelectedWpn);
			delayBtwChangingWeapon.restart();
		}
		// je�eli wci�ni�to B to bierzemy poprzedni� bro�
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			this->indexOfCurrentlySelectedWpn -= 1;
			roundValue(this->backpack.size(), this->indexOfCurrentlySelectedWpn);
			delayBtwChangingWeapon.restart();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::changeToCity()
{
	this->isPlayingAnimation = true;
	inWhichRoomWeAre = statesOfShip::MAINMENU;
	playAnimation();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::playAnimation()
{
	// (!)
	// Uruchamiam animacj� zwyci�stwa !
	if (this->howManyCreaturesLeft == 0)
	{
		victoryText.setCharacterSize(96.f);
		victoryText.setFont(Font);
		victoryText.setPosition(sf::Vector2f(xViewOffset + WINDOWWIDTH / 16, yViewOffset + WINDOWHEIGHT / 3));
		victoryText.setString("Zwyci�stwo !");
		victoryText.setFillColor(sf::Color::Color(0x80, 0x80, 0x0, 0x0));
		this->alpha = 0;
		this->step = 255 / 2.f / FPS;
		// puszczamy animacj� zwyci�stwa 
		this->timeOfAnimation.restart();
		while (alpha <= 255)
		{
			this->Window.clear();

			sf::Event event;
			while (this->Window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					exit(0);
				}
			}

			
			// dajemy mo�liwo�� poruszania si� graczem i strzelania podczas animacji 
			// (�eby si� nacieszy� �e wygra�o si� fal� :> )
			Window.draw(Player);
			updateAnimation(true);

			this->Window.display();
		}

		this->initializeMainMenuShipState();
		this->isPlayingAnimation = false;

		this->currentState = statesOfPlayGameState::SHIP;
		this->Window.clear();

	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::updateAnimation(bool managePlayer)
{
	Window.draw(Player.getCircle());

	if (DEBUG)
		Window.draw(Player.getLine());

	// rysowanie pocisk�w
	for (int i = 0; i < ProjectilesArray.size(); ++i)
		Window.draw(ProjectilesArray[i]);

	// usuwanie zbednych pocisk�w
	handleErasingProjectiles();

	// zarz�dzanie strzelaniem
	handleShooting();

	if (managePlayer)
	{
		Player.update(sf::Mouse::getPosition(this->Window));
	}


	// zarz�dzanie widokiem 
	manageView();

	// zwi�kszamy opacity 
	this->alpha += this->step;
	victoryText.setFillColor(sf::Color::Color(0x80, 0x80, 0x0, alpha));
	Window.draw(victoryText);


}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleSettingUpWave()
{
	// opis stanu :
	// podczas stanu gracz mo�e si� porusza� i strzela� (ale w sumie po choler� bo nie ma do czego strzela� :/  )
	// jest te� wy�wietlony olbrzymi licznik na ca�y ekran kt�ry odlicza czas do rozpocz�cia fali 
	// po tym jak dobije do 0, losujemy spawn pointy i przechodzimy w stan PLAYINGWAVE

	// update stanu
	updateSettingUpWave();

	// zarz�dzanie licznikiem
	handleSettingUpWaveCounter();

	// wy�wietlanie rzeczy do fali
	displaySettingUpWave();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::updateSettingUpWave()
{
	sf::Event event;
	while (this->Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			exit(0);
		}
	}

	// zarz�dzanie widokiem
	manageView();

	Player.update(sf::Vector2i(sf::Mouse::getPosition(this->Window).x + xViewOffset, sf::Mouse::getPosition(this->Window).y + yViewOffset));

	handleErasingProjectiles();


	// zarz�dzanie strzelaniem
	handleShooting();
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::displaySettingUpWave()
{

	this->Window.clear();


	// rysowanie naszego gracza 
	this->Window.draw(Player.getCircle());
	this->Window.draw(Player);
	this->Window.draw(Player.getLine());


	for (int i = 0; i < ProjectilesArray.size(); ++i)
		this->Window.draw(ProjectilesArray[i]);

	for (int i = 0; i < ammoAndCurrencyTexts.size(); ++i)
		this->Window.draw(ammoAndCurrencyTexts[i]);


	this->Window.draw(counter);

	this->Window.display();


}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::handleSettingUpWaveCounter()
{
	int counterNum = DELAYBTWWAVES - static_cast<int>(GETSEC(SettingUpWaveTimeElapsed));

	counter.setPosition(xViewOffset + WINDOWWIDTH / 2 - counter.getCharacterSize() / 2, yViewOffset + WINDOWHEIGHT / 2 - counter.getCharacterSize());
	counter.setString(std::to_string(counterNum));


	// je�eli counter stanie si� 0 to generujemy spawnPoint'y i zmieniamy stan
	if (!counterNum)
	{
		// (!) trzeba wymy�li� tutaj jaki� m�dry algorytm kt�ry sensownie b�dzie generowa� ilo�� spawnPoint�w
		int howManySpawnPoints = rand() % 5 + 1;
		generateSpawnPoint(howManySpawnPoints);
		this->timeElapsedWave.restart();
		this->currentState = statesOfPlayGameState::PLAYINGWAVE;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
std::string playGameState::filtrateString(std::string &line)
{
	// metoda kt�ra z podanego napisu odczyta ca�y tekst do znaku komentarza 

	std::string phrase;
	// przechodzimy przez ca�� linijk� i pobieramy sobie liczby

	for (int j = 0; j < line.size(); ++j)
	{
		if (line[j] == commentChar)
			break;
		phrase += line[j];

	}

	return phrase;
}

/////////////////////////////////////////////////////////////////////////////////////////////
bool playGameState::worthAddingToVec(std::string & line)
{
	if (!line.size())
		return false;


	// do wektora b�dziemy wrzuca� tylko te stringi kt�re nie b�d� puste

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
splatTemplate playGameState::getRandomSplatTemplate()
{
	int sum = 0;
	int randomVal = rand() % this->allChance;

	for (int i = 0; i < splatTemplateDatabase.size(); ++i)
	{
		if (randomVal < splatTemplateDatabase[i].chanceOfGettingRandomized + sum)
		{
			return splatTemplateDatabase[i];
		}
		sum += splatTemplateDatabase[i].chanceOfGettingRandomized;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
void playGameState::manageAmmoAndCurrencyTexts()
{
	assert(ammoAndCurrencyTexts.size()  != 0);
	ammoAndCurrencyTexts[0].setString(std::to_string(Ammunitions[m_CurrSelectedWpn.getType()]));
	ammoAndCurrencyTexts[1].setString(std::to_string(m_CurrSelectedWpn.howManyBulletsLeftInMag()));
	ammoAndCurrencyTexts[2].setString("$ : " + std::to_string(this->currency));

	if (previousMagSize != m_CurrSelectedWpn.howManyBulletsLeftInMag())
	{
		int RCol, GCol;
		// chcemy by tekst kt�ry wy�wietla aktualn� ilo�� amunicji w magazynku w zale�no�ci od wystrzelonych kulek przechodzi� z zielonego na czerwony 
		//																			(zielony = pe�ny magazynek, czerwony = pusty magazynek)
		
		RCol = (1.0 - cast(double, m_CurrSelectedWpn.howManyBulletsLeftInMag()) / cast(double, m_CurrSelectedWpn.getMaxMagSize())) * 255;
		GCol = 255 - RCol;
		ammoAndCurrencyTexts[1].setFillColor(sf::Color::Color(RCol, GCol, 0x0));
	}
	if (prevWpnType != m_CurrSelectedWpn.getType())
	{
		ammoAndCurrencyTexts[0].setFillColor(colorMap[m_CurrSelectedWpn.getType()]);
	}




	this->prevWpnType		=	 m_CurrSelectedWpn.getType();
	this->previousMagSize	=	 m_CurrSelectedWpn.howManyBulletsLeftInMag();
}

/////////////////////////////////////////////////////////////////////////////////////////////