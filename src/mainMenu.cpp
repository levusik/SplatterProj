#include "mainMenu.h"
#pragma once

bool intersectWithMouse(sf::RenderWindow &window, sf::FloatRect &globalBounds);

mainMenuState::mainMenuState(sf::Font &font)
{
	result = -1;
	needToRedraw = true;

	//w konstruktorze ustawiamy napisy oraz teksty
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(96.f);


	// =zarz¹dzanie kolorkami
	defaultColor = sf::Color::Color(0xB0, 0x0, 0x0);
	textHighlighted = sf::Color::Color(0x0, 0xBF, 0x0);

	text.setFillColor(defaultColor);

	text.setString("Graj!");
	texts.push_back(text);

	text.setString("Wczytaj");
	texts.push_back(text);

	text.setString("Zakoñcz");
	texts.push_back(text);

	// ustawienie pozycji
	for (int i = 0; i < texts.size(); ++i)
		texts[i].setPosition(WINDOWWIDTH / 8, WINDOWHEIGHT / 16 + i * 96);



}
int mainMenuState::runState(sf::RenderWindow &window)
{
	while (result == -1)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				exit(0);
			}
		}

		manageHumanInput(window);
		// czyœcimy i rysujemy tylko i wy³¹cznie wtedy kiedy trzeba 
		if (needToRedraw)
		{
			window.clear();
			display(window);
			window.display();
			needToRedraw = false;
		}
		// w przeciwnym razie pomijamy klatkê 
		else
		{
			Sleep(1000 / FPS);
		}
	}
	return result;
}
void mainMenuState::display(sf::RenderWindow &window)
{
	for (int i = 0; i < texts.size(); ++i)
	{
		window.draw(texts[i]);
	}
	for (int i = 0; i < sprites.size(); ++i)
	{
		window.draw(sprites[i]);
	}
}
void mainMenuState::manageHumanInput(sf::RenderWindow &window)
{
	for (int i = 0; i < texts.size(); ++i)
	{
		if (intersectWithMouse(window, texts[i].getGlobalBounds()))
		{
			needToRedraw = true;
			texts[i].setFillColor(textHighlighted);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				result = i;
			}
		}
		else if (texts[i].getFillColor() == textHighlighted)
		{
			needToRedraw = true;
			texts[i].setFillColor(defaultColor);
		}
	}
}
bool intersectWithMouse(sf::RenderWindow &window, sf::FloatRect &globalBounds)
{
	if (sf::Mouse::getPosition(window).x > globalBounds.left &&
		sf::Mouse::getPosition(window).x < globalBounds.left + globalBounds.width
		&& sf::Mouse::getPosition(window).y > globalBounds.top &&
		sf::Mouse::getPosition(window).y < globalBounds.height + globalBounds.top)
		return true;
	return false;
}