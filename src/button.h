#pragma once
#include <SFML\Graphics.hpp>
#include "entity.hpp"


// robimy w³asnego guzika bo z winApi mnie szlag trafia a 
// Qt jeszcze siê uczê :|
class Button : public sf::RectangleShape
{
public:
	Button(usageOfButton usage);
	Button(usageOfButton usage, sf::Vector2f size, sf::Vector2f position, bool isInteractable);


	bool update(sf::RenderWindow &window);
	usageOfButton getUsage();

	// settery
	void setColors(sf::Color colorWhenPassive, sf::Color colorWhenHighlighted);
	void setSpriteTexture(sf::Texture &texture);
	void setText(std::string phrase, sf::Font &font);
	void draw(sf::RenderWindow &window);

	// gettery
	sf::Sprite getSprite() const;
	sf::Text   getText()   const;


private:

	bool			isInteractable;
	usageOfButton	usageOfThisButton;
	sf::Color		colorWhenPassive;
	sf::Color		colorWhenHighlighted;
	sf::Sprite		sprite;
	sf::Text		text;
};