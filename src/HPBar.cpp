#include "HPBar.h"
#include "enemy.hpp"
#include "entity.hpp"
#include <iostream>

void HPbar::init(double maxHP, double widthSize, double HeightSize, sf::Color startingColor)
{
	this->maxHP = maxHP;
	this->startingWidth = widthSize;
	this->height = HeightSize;

	this->setSize(sf::Vector2f(widthSize, HeightSize));
	this->setPosition(sf::Vector2f(0, 0));

	this->startingRCol = startingColor.r;
	this->startingGCol = startingColor.g;
	this->startingBCol = startingColor.b;
	this->setFillColor(startingColor);
}


void HPbar::update(sf::Vector2f position, double actualHP, bool centerRectangle, double radius)
{
	double HpRadio = actualHP / this->maxHP;
	double actualSize = HpRadio * startingWidth;


	// ustawiamy pozycj� 
	if (position.x != this->getPosition().x || position.y != this->getPosition().y)
	{
		sf::Vector2f pos = sf::Vector2f(position.x, position.y);

		if (centerRectangle)
			pos = sf::Vector2f(pos.x - actualSize / 2, pos.y - radius - 2 * height);

		this->setPosition(pos);
		
	}
	// ustawienie kolork�w
	// wyja�nienie :
	// chcemy by kolor tego paska zmenia� si� w zale�no�ci od ActualHP/maxHP.
	//
	//		Bo rysunek wyra�a wi�cej ni� 1000 s��w
	//	
	//	|-----------------------|------------------------|------------------------|
	//	|			je�eli zdrowie ro�nie			je�eli zdrowie maleje		  |
	//	|	Startowy Kolor	  <----	  aktualnyKolor    ---->	maks. czerwony	  |	
	//	|												 						  |
	//	|-----------------------|------------------------|------------------------|
	//
	sf::Color newColor = sf::Color( ( 1. - HpRadio)*(0xff- this->startingRCol) + this->startingRCol  , HpRadio * this->startingGCol, HpRadio * this->startingBCol);


	if ( newColor != this->getFillColor()) 
		this->setFillColor(newColor);

	// ustawienie rozmiaru
	if (static_cast<float>(HpRadio * startingWidth) != this->getSize().x)
		this->setSize(sf::Vector2f(actualSize, this->getSize().y));
}