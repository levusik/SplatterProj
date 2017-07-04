#include "specialProjectiles.h"
#include <iostream>

/**********************************************************/
/*				odbijaj¹cy siê pocisk					  */
/**********************************************************/
void flexibleBullet::initializeSpecialEffects()
{
	maxTimeOfProjection = 30.f;

}

/**********************************************************/
void flexibleBullet::manageSpecialEffects()
{
	if (getPosition().x <= LEFTWALL + std::abs(Vx) || getPosition().x + 2*getRadius() >= RIGHTWALL - std::abs(Vy))
	{
		Vx *= -1;
	}
	if (getPosition().y <= UPWALL || getPosition().y + 2 * getRadius() >= DOWNWALL)
	{
		Vy *= -1;
	}
}
/**********************************************************/
bool flexibleBullet::isSmartBullet()
{
	return true;
}

/**********************************************************/


/**********************************************************/
/*				inteligentne pociski					  */
/**********************************************************/
void smartBullet::initializeSpecialEffects()
{
	this->bulletDestination = sf::Vector2f(0, 0);

	this->originalVX = std::abs(Vx);
	this->originalVY = std::abs(Vy);

	this->maxTimeOfProjection = 10.f;
}
/**********************************************************/
void smartBullet::manageSpecialEffects()
{
	double xPos = bulletDestination.x  - this->getCenter().x;
	double yPos = bulletDestination.y  - this->getCenter().y ;

	double distanceBtwBltAndEnemy = std::sqrt(xPos * xPos + yPos * yPos);

	double sinA = yPos/distanceBtwBltAndEnemy;
	double cosA = xPos/distanceBtwBltAndEnemy;


	this->Vx = cosA * (originalVX + std::abs(Vx))/2;
	this->Vy = sinA * (originalVY + std::abs(Vy))/2;
}
bool smartBullet::isFlexibleBullet()
{
	return true;
}
/**********************************************************/
