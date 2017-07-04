#include "specialAbility.h"

/***************************************************************************************************/
specialAbility::specialAbility()
{
	this->isActive = false;
	this->type = typeOfSpecialAbility::VOID;
	this->DamageParameters.value = 0;
	this->enemyArrayPtr = NULL;
}
/***************************************************************************************************/
void specialAbility::setRectAndSpriteParams(sf::Vector2f size, sf::Vector2f position,
											sf::Color col)
{
	rect.setSize(size);
	rect.setPosition(position);
	rect.setFillColor(col);

	this->setPosition(position);
}
/***************************************************************************************************/
void specialAbility::setTexture(sf::Texture &texture)
{
	this->setTexture(texture);
}
/***************************************************************************************************/
void specialAbility::setPointerToEnemyArray(std::vector<std::vector<enemy*>> *enemyArrPtr)
{
	this->enemyArrayPtr = enemyArrayPtr;
}
/***************************************************************************************************/
void specialAbility::setPointerToWindow(sf::RenderWindow * windowPtr)
{
	this->windowPtr = windowPtr;
}
/***************************************************************************************************/
bool specialAbility::isStillActive()
{
	return this->isActive;
}
/***************************************************************************************************/
sf::RectangleShape specialAbility::getRect() const {
	return rect;
}
/***************************************************************************************************/