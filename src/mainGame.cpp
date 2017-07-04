#include "mainGame.h"

mainGame::mainGame()
{
	// ustawienie okienka
	window.create(sf::VideoMode(WINDOWWIDTH, WINDOWHEIGHT), "Splatter!");
	window.setFramerateLimit(30);


	// pocz�tkowy stan maszyny
	currentStateOfMachine = statesOfStateMachine::mainMenu;
	whatToDoAfterMainMenu = -1;


	// TODO (!) zrobi� sensowny system wyj�tk�w

	if (!font.loadFromFile("space age.ttf"))
	{
		MessageBox(NULL, "Nie uda�o si� za�adowa� czcionki!", NULL, NULL);
	}

}

void mainGame::run()
{
	while (true)
	{
		mainMenuState *MainMenu = new mainMenuState(font);
		whatToDoAfterMainMenu = MainMenu->runState(window);

		switch (whatToDoAfterMainMenu)
		{
		case 0: 	// Graj 
			playGame();
			break;
		case 1:		// wczytaj
			// (!)
			break;
		case 2:		// zako�cz
			exit(0);
			break;

		}

		delete MainMenu;
	}

}

void mainGame::playGame()
{
	playGameState *newGame = new playGameState(font, this->window);
	newGame->run();
}
