#pragma once

#include <SFML\Graphics.hpp>
#include "entity.hpp"

// pociski kt�re odbijaja si� od �cian
class flexibleBullet : public projectile
{
	virtual void initializeSpecialEffects() override;
	virtual void manageSpecialEffects()		override;
	virtual bool isSmartBullet()			override;


protected:
	double	  timeToExplode;
	bool	  hasExploded;
};


class smartBullet : public projectile
{
protected:
	virtual void initializeSpecialEffects() override;
	virtual void manageSpecialEffects()		override;
	virtual bool isFlexibleBullet()			override;

	double originalVX, originalVY;
	sf::Clock delayBtwSettingNewPosition;
};