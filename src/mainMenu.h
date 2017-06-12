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

	// napisy które bêd¹ wyœwietlane podczas g³ównej pêtli stanu
	std::vector<sf::Text> texts;

	// sprajty które bêd¹ wyœwietlanie podczas g³ównej petli stanu
	std::vector<sf::Sprite> sprites;

	// bool lock który bêdzie kontrolowaæ czy musimy wyczyœciæ i narysowaæ ponownie 
	bool needToRedraw;

	// koñcowy wynik stanu : 
	// 0 - wyjœcie 
	// 1 - przejœcie w stan "Graj"
	// 2 - przejœcie w stan "Wczytaj"
	int result;

	// kolorki ! 
	sf::Color defaultColor, textHighlighted;
};