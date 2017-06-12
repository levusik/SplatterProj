#include "button.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Button::Button(usageOfButton usage)
{
	this->usageOfThisButton = usage;

	// uzupe�nienie domy�lnymi warto�ciami
	this->setPosition(sf::Vector2f(0, 0));
	this->setSize(sf::Vector2f(64, 64));

	this->colorWhenHighlighted = sf::Color::Color(0xA0, 0xA0, 0x0);
	this->colorWhenPassive = sf::Color::Color(0xff, 0x0, 0x0);

	this->setFillColor(this->colorWhenPassive);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Button::Button(usageOfButton usage, sf::Vector2f size, sf::Vector2f position, bool isInteractable)
{

	this->isInteractable = isInteractable;
	this->usageOfThisButton = usage;
	this->setPosition(position);
	this->setSize(size);

	this->colorWhenHighlighted = sf::Color::Color(0xA0, 0xA0, 0x0);
	this->colorWhenPassive = sf::Color::Color(0xff, 0x0, 0x0);

	this->setFillColor(this->colorWhenPassive);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// funkcja update'uj�ca guzik
// kt�ra zwraca 1 je�eli guzik zosta� klikni�ty lub 0 je�eli nic ciekawego si� nie sta�o
bool Button::update(sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	if (isInteractable)
	{
		if (mousePos.x >= this->getGlobalBounds().left && mousePos.x <= this->getGlobalBounds().left + this->getGlobalBounds().width
			&& mousePos.y >= this->getGlobalBounds().top && mousePos.y <= this->getGlobalBounds().top + this->getGlobalBounds().height)
		{
			// zabezpieczamy si� przed ci�g�ym zmienianiem koloru
			if (this->getFillColor() != this->colorWhenHighlighted)
			{
				this->setFillColor(colorWhenHighlighted);
			}
			// zwracamy sygna� �e klikni�to guzik
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				return true;
			}


		}
		else if (this->getFillColor() != this->colorWhenPassive)
		{
			this->setFillColor(colorWhenPassive);
		}
	}
	window.draw(this->text);
	window.draw(this->sprite);


	return false;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// zabawa kolorkami
void Button::setColors(sf::Color colorWhenPassive, sf::Color colorWhenHighlighted)
{
	this->colorWhenPassive = colorWhenPassive;
	this->colorWhenHighlighted = colorWhenHighlighted;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::setSpriteTexture(sf::Texture & texture)
{
	this->sprite.setTexture(texture);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::setText(std::string phrase, sf::Font & font)
{

	this->text.setPosition(this->getPosition().x + this->getGlobalBounds().width/8, this->getPosition().y + this->getGlobalBounds().height / 6);
	this->text.setCharacterSize(this->getGlobalBounds().width / phrase.size());
	this->text.setFillColor(sf::Color::Color(0xA0,0xA0,0x0));
	this->text.setString(phrase);
	this->text.setFont(font);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::draw(sf::RenderWindow &window)
{
	window.draw(*this);
	window.draw(this->sprite);
	window.draw(this->text);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sf::Sprite Button::getSprite() const
{
	return this->sprite;
}
sf::Text Button::getText() const
{
	return this->text;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
usageOfButton Button::getUsage()
{
	return this->usageOfThisButton;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////