#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "entity.hpp"
#include "mainGame.h"
#include "mainMenu.h"
#include "playGameState.h"



class mainGame
{
public:
	mainGame();
	void run();

private:
	sf::RenderWindow window;

	statesOfStateMachine currentStateOfMachine;
	int whatToDoAfterMainMenu;
	sf::Font font;
	void playGame();

};