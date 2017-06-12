#pragma once
#include <SFML\Graphics.hpp>
#include "entity.hpp"
#include <cstdlib>
#include <Windows.h>

class mainMenuState
{
public:
	mainMenuState(sf::Font &font);
	int runState(sf::RenderWindow &window);

private:
	void display(sf::RenderWindow &window);
	void manageHumanInput(sf::RenderWindow &window);

	// napisy kt�re b�d� wy�wietlane podczas g��wnej p�tli stanu
	std::vector<sf::Text> texts;

	// sprajty kt�re b�d� wy�wietlanie podczas g��wnej petli stanu
	std::vector<sf::Sprite> sprites;

	// bool lock kt�ry b�dzie kontrolowa� czy musimy wyczy�ci� i narysowa� ponownie 
	bool needToRedraw;

	// ko�cowy wynik stanu : 
	// 0 - wyj�cie 
	// 1 - przej�cie w stan "Graj"
	// 2 - przej�cie w stan "Wczytaj"
	int result;

	// kolorki ! 
	sf::Color defaultColor, textHighlighted;
};