#include "entity.hpp"
#include <iostream>

namespace tEffctsConst
{
	/*-------------------------------------------------------*/
	// obra¿enia poszczególnych obszarówek
	const double explodingBulletsDmgDebuff	= 0.5f;
	const double burningBulletsDmgDebuff		= 0.3f;
	const double electrizeBulletsDmgDebuff	= 0.2f;
	const double toxicateBulletsDmgDebuff	= 0.1f;
	/*-------------------------------------------------------*/


	/*-------------------------------------------------------*/
	// maksymalny i minimalny czas trwania obszarówek
	const int explodingBulletsMaxTime	= 2;
	const int explodingBulletsMinTime	= 1;

	const int burningBulletsMaxTime		= 4;
	const int buriningBulletsMintime	= 2;

	const int electrizeBulletsMaxTime	= 5;
	const int electrizeBulletsMinTime	= 4;

	const int toxicateBulletsMaxTime	= 6;
	const int toxicateBulletsMinTime	= 4;
	/*-------------------------------------------------------*/
	//	 maksymalny rozmiar wybuchu
	const double explodingBulletRadiusMultipler = 4.;
	const double burningBulletRadiusMultipler	= 2.;
	const double electrizeBulletRadiusMultipler = 2.;
	const double toxicateBulletRadiusMultipler	= 2.5;

	/*-------------------------------------------------------*/
	double simpleFunc(double counter, int time)
	{
		return pow(sin(radToDgr(2 * counter) * M_PI / time),0.5);
	}


}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//		KONSTRUKTOR
projectile::projectile()
{
	this->EfctsCausedByBlt = NULL;
	this->animCounter = this->actualOrigRadiusSize = this->origRadiusSize = this->colorCounter = 0;
	this->step = 1;
	this->isSmartBulletBoolean = 0;

	this->bulletDestination = sf::Vector2f(0, 0);
	this->func = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
projectile::~projectile()
{
	delete this->EfctsCausedByBlt;
	EfctsCausedByBlt = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::setFunctionPtr(double(*ptr)(double), double step)
{
	this->func = ptr;
	this->funcStep = step;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::create(sf::Vector2f playerPos, double radians, rangeWeapon *weapon, int iter)
{
	// po 2.5 sekundach ka¿dy pocisk zostaje usuniêty ¿eby nie obci¹¿aæ zbytnio procka
	timeOfProjection.restart();
	maxTimeOfProjection = min(1200 / (abs(this->Vy) + abs(Vy)) / 2, 10.f);
	this->EfctsCausedByBlt = &weapon->EffectWeaponCauses;
	this->isSmartBulletBoolean = weapon->hasSmartBullets();


	// metoda która w zale¿noœci od broni odpowiednio wyreguluje prêdkoœæ, kierunek, itp.
	this->manageEffectsInit(*weapon, radians, iter);


	// ustawienie kó³eczka 
	this->setFillColor(sf::Color::Red);
	this->setRadius(weapon->bulletSize);
	this->setPosition(playerPos);
	this->setOrigin(this->getRadius(), this->getRadius());




	this->origRadiusSize = this->getRadius();
	this->NeedToDeleteBoolean = false;


	if (this->EfctsCausedByBlt->a_bullThatBurns)
		this->intializeTerrainEffects	(tEffctsConst::burningBulletsDmgDebuff, tEffctsConst::burningBulletsMaxTime,
										  tEffctsConst::buriningBulletsMintime, tEffctsConst::simpleFunc,
										  tEffctsConst::burningBulletRadiusMultipler,1, typeOfTerrainEffects::FIRE);

	else if (this->EfctsCausedByBlt->a_bullThatElectrize)
		this->intializeTerrainEffects(tEffctsConst::electrizeBulletsDmgDebuff, tEffctsConst::electrizeBulletsMaxTime,
									  tEffctsConst::electrizeBulletsMinTime, tEffctsConst::simpleFunc,
									  tEffctsConst::electrizeBulletRadiusMultipler, 1, typeOfTerrainEffects::ELECTRIC);

	else if (this->EfctsCausedByBlt->a_bullThatToxicate)
		this->intializeTerrainEffects(tEffctsConst::toxicateBulletsDmgDebuff, tEffctsConst::toxicateBulletsMaxTime,
									  tEffctsConst::toxicateBulletsMinTime, tEffctsConst::simpleFunc,
									  tEffctsConst::toxicateBulletRadiusMultipler, 1, typeOfTerrainEffects::TOXIC);


	else if (this->EfctsCausedByBlt->a_bullThatExplode)
		this->intializeTerrainEffects(tEffctsConst::explodingBulletsDmgDebuff, tEffctsConst::explodingBulletsMaxTime,
									  tEffctsConst::explodingBulletsMinTime, tEffctsConst::simpleFunc,
									  tEffctsConst::explodingBulletRadiusMultipler, 1, typeOfTerrainEffects::EXPLOSIVE);

	initializeSpecialEffects();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::update()
{
	this->newRadius = func(this->funcCounter) * this->origRadiusSize;
	this->funcCounter += this->step;

	this->setRadius(newRadius);

	if (EfctsCausedByBlt != NULL)
	{
		switch (this->EfctsCausedByBlt->type)
		{
		case typeOfWeapon::ENERGETIC:
		{
			this->handleEnergeticAnimations();
			break;
		}
		case typeOfWeapon::PLASMA:
		{
			this->handlePlasmaAnimations();
			break;
		}

		}
		this->prevRadiusSize = this->getRadius();

		if (GETSEC(this->timeOfProjection) >= this->maxTimeOfProjection)
		{
			NeedToDeleteBoolean = true;
		}
	}

	manageSpecialEffects();

	// po prostu update
	this->move(this->Vx  * GETSEC(timeBasedMovement), this->Vy * GETSEC(timeBasedMovement));
	timeBasedMovement.restart();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::setSmartBulletDestination(sf::Vector2f position)
{
	this->bulletDestination = position;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
double projectile::getRandomVal(int minVal, int maxVal)
{
	double val = (rand() % ((maxVal - minVal) + 1)) + minVal;
	return val;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
sf::Vector2f projectile::getCenter()
{
	return sf::Vector2f(this->getPosition().x, this->getPosition().y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
effects projectile::getEffects()
{
	return *this->EfctsCausedByBlt;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
double projectile::getDmg()
{
	return this->damage;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
sf::Time projectile::getTime()
{
	return this->timeOfProjection.getElapsedTime();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
double projectile::getMaxTime()
{
	return this->maxTimeOfProjection;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
terrainEffectParams projectile::getTerrainEffects()
{
	return this->TerrainEffectsParameters;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
damageParameters projectile::getDamageParameters()
{
	damageParameters params;
	params.value = this->damage;

	return params;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::intializeTerrainEffects(double dmgDebuff, int maxTime, int minTime, double (*funcPtr)(double,int),double radMultipler, int step, typeOfTerrainEffects type)
{
	//obra¿enia zadawane przez obszarówkê maj¹ byæ jak¹œ czêœci¹ obra¿eñ zadanych przez pocisk
	TerrainEffectsParameters.damage = this->damage * dmgDebuff;

	//
	TerrainEffectsParameters.durationOfAnimation = rand() % (maxTime - minTime) + 1 + minTime;
	TerrainEffectsParameters.function = funcPtr;
	
	if (this->EfctsCausedByBlt->a_manualTerrainDamageRadiusSize < 0 )
		TerrainEffectsParameters.maxRadiusSize = rand() % cast(int, this->getRadius()*(radMultipler - 1) + 1) + this->getRadius();

	TerrainEffectsParameters.step = step;
	TerrainEffectsParameters.type = type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool projectile::needToDelete()
{
	return NeedToDeleteBoolean;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool projectile::isSmartBullet()
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::manageEffectsInit(rangeWeapon weapon, double radians, int iter)
{
	// je¿eli jest to pierwszy pocisk z 'serii' to generujemy nowe dane
	// natomiast jeœli jest to kolejny pocisk chcemy by mia³ on podobne dane do poprzedniego 
		this->recoilValue = rand() % static_cast<int>(weapon.recoil) + 1;
		this->Vx = std::abs(this->getRandomVal(weapon.minSpeedOfBullet, weapon.maxSpeedOfBullet));
		this->Vy = std::abs(this->getRandomVal(weapon.minSpeedOfBullet, weapon.maxSpeedOfBullet));
		this->damage = getRandomVal(weapon.minDamage, weapon.maxDamage);


	// metoda która zajmuje siê ogarniêciem efektów z prefixem a_... (czyli te dla wszystkich a.k.a. celowniki itp.)
	handleCommonEffects(weapon);

	// w zale¿noœci od tego jaki typ kulki to jest to odpowiednio to modyfikujemy
	switch (this->EfctsCausedByBlt->type)
	{
	case typeOfWeapon::PLASMA:
		this->managePlasmaEffectsinit();
		break;
	case typeOfWeapon::BULLETTYPE:
		this->manageBulletTypeEffectsInit();
		break;
	case typeOfWeapon::ENERGETIC:
		this->manageEnergeticEffectsInit();
		break;
	}

	// ustawiamy kierunek lotu 		
	// algorytm tworzenia pocisków 
	// to bêdzie ciekawe :)
	// angle = player.angle + (-1)^i*i*weapon.recoil

	this->Vx *= cos(radians + (std::pow(-1,iter) * static_cast<double>(iter)/4 * recoilValue    * M_PI / 180));
	this->Vy *= sin(radians + (std::pow(-1, iter) * static_cast<double>(iter) / 4 * recoilValue * M_PI / 180));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::managePlasmaEffectsinit()
{
	if (EfctsCausedByBlt->p_plasmaMagicBullets)
	{

	}


	this->Vx *= g_plasmaSpeedBuff;
	this->Vy *= g_plasmaSpeedBuff;
	this->damage *= (1.0 / g_plasmaSpeedBuff);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::manageEnergeticEffectsInit()
{
	

	this->Vx *= g_energeticSpeedBuff;
	this->Vy *= g_energeticSpeedBuff;
	this->damage *= (1.0 / g_energeticSpeedBuff);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::manageBulletTypeEffectsInit()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::handleCommonEffects(rangeWeapon weapon)
{
	if (EfctsCausedByBlt->a_hasScope)
	{
		this->recoilValue /= 2;
	}
	if (EfctsCausedByBlt->a_hasStabilizator)
	{
		this->recoilValue /= 2;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::handleEnergeticAnimations()
{
	// wprowadzamy chaos do broni energetycznej
	double subtract = newRadius - this->prevRadiusSize;
	
	this->move(-subtract, -subtract);
	
	// magic number : kolorki !
	colorCounter += rand() % 5 + 3;
	colorCounter %= 360;
	this->setFillColor(sf::Color(0, 0x30 + abs(sin(radToDgr(colorCounter ))) * 0xcf, 0x40 + 0xbf *  abs(cos(radToDgr(colorCounter)))  ));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void projectile::handlePlasmaAnimations()
{
	// broñ plazmowa mniej chaotyczna ni¿ broñ energetyczna 
	double subtract = newRadius - this->prevRadiusSize;

	this->move(-subtract, -subtract);
	

	this->colorCounter += rand() % 3 + 2;
	this->setFillColor(sf::Color(0, 0x40 + 0xbf * abs(cos(radToDgr(colorCounter))),  0x40 * abs(sin(radToDgr(colorCounter)) )));
	this->colorCounter %= 360;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool projectile::isFlexibleBullet()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////