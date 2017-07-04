#include "entity.hpp"
#include "specialProjectiles.h"
#include <iostream>

namespace projectileFunctions
{
	/*--------------------------------------------------------------------*/
	//		 k�cik animacji broni o�owiowych
	//		 sta�e wykresy
	int howManyLeadAnimations = 1;
	double leadAnimation(double counter)
	{
		return 1 - abs(sin( radToDgr(counter) )) * .15;
	}
	/*--------------------------------------------------------------------*/
	//		k�cik animacji broni plazmowych
	//		"wolne wykresy" : wzrastaj� i malej� do�� wolno w stosunku do x
	int howManyPlasmaAnimations = 5;
	double plasmaAnimation1(double counter)
	{
		return std::pow(sin(radToDgr(counter * 2)), 2) * abs(pow(cos(radToDgr(0.1 * counter)), 3)) + 1;
	}
	double plasmaAnimation2(double counter)
	{
		return pow(sin(radToDgr(2*counter)), 2) * abs(pow(cos(radToDgr(0.1* counter)),3)) + 1;
	}
	double plasmaAnimation3(double counter)
	{
		return 1 + abs(sin(radToDgr(4 * counter)))* 0.7;
	}
	double plasmaAnimation4(double counter)
	{
		return 1. / (abs(sin(radToDgr(3 * counter))) + 1) + 1;
	}
	double plasmaAnimation5(double counter)
	{
		return cos(radToDgr(5 * counter)) * .2 + 2 / (abs(cos(radToDgr(counter))) + 1);
	}
	/*--------------------------------------------------------------------*/
	//		k�cik animacji broni energetycznych
	//		"dynamiczne wykresy" : wzrastaj� i malej� raczej szybko w stosunku do x
	int howManyEnergeticAnimations = 5;
	double energeticAnimation1(double counter)
	{
		// wykres : sin(20x)/(|cos(x)|+1) *.5 + 1
		return sin(radToDgr(20 * counter)) / (abs(cos(radToDgr(counter))) + 1) * .5 + 1;
	}
	double energeticAnimation2(double counter)
	{
		return cos(radToDgr(20*counter)) / pow((abs(sin(radToDgr(counter))) + 1), 2) * .5 + 1;
	}
	double energeticAnimation3(double counter)
	{
		return sin(radToDgr(30*counter)) / (abs(cos(radToDgr(counter))) + 1) * .5 + 1;
	}
	double energeticAnimation4(double counter)
	{
		// wykres : y = cos(0.1x/( | sin(x) |+ 1 )^2 * 0.5 + 1
		return cos((radToDgr(0.5*counter)) / (pow(abs(sin(radToDgr(counter))) + 1, 2))) * 0.5 + 1;
	}
	double energeticAnimation5(double counter)
	{
		// wykres : y = | sin(ln(x)/ (|cos(x)|+1)^2 ) |* .5 + 1
		return sin(radToDgr(log(2*counter) / pow((abs(cos(counter * M_PI / 180.))) + 1, 2)))*.5 + 1;
	}
	/*--------------------------------------------------------------------*/
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
rangeWeapon::rangeWeapon()
{
	this->clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::setEffects(effects efcsCausedByWpn)
{
	this->EffectWeaponCauses = efcsCausedByWpn;
	this->initializeEffects();
}

void rangeWeapon::initializeEffects()
{
	if (this->EffectWeaponCauses.e_isCharger)
		this->reloadingTime *= 10;

	if (this->EffectWeaponCauses.a_hasExtendedMag)
		this->origMagSize *= 2;

	if (this->EffectWeaponCauses.l_isFlameThrower)
	{
		this->EffectWeaponCauses.a_bullThatBurns = true;
		this->firerate = 4000;
		this->maxDamage /= 4;
		this->minDamage /= 4;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::takeAction(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile*>& ProjectilesArray, playerParameters PlayerParams)
{
	
	if (this->isReloading == true)
	{
		// je�eli prze�adowujemy to czekamy na moment w kt�rym zostanie zako�czone prze�adowanie 
		handleReloading(Ammunitions, PlayerParams);
	}
	else
	{
		// prze�adowanie wywo�ane przez gracza
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			this->tryReloading(Ammunitions);
		}

		
		// strzelanie 
		this->manageShooting(Ammunitions, ProjectilesArray, PlayerParams);


		// sprawdzamy czy nie trzeba przypadkiem prze�adowa� 
		if (this->actualMagSize < std::pow(this->howManyProjItShoot, negate(this->packedBullets)) * this->costOfBullet && this->delayBtwAutoReloading.getElapsedTime().asSeconds() > 0.5f)
		{
			tryReloading(Ammunitions);
			this->delayBtwAutoReloading.restart();
		}

	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::handleReloading(std::map<typeOfWeapon, int>& Ammunitions, playerParameters playerParams)
{
	if (this->timeElpasedForReloading.getElapsedTime().asSeconds() > this->reloadingTime)
	{

		// zwracamy kulki 
		Ammunitions[this->getType()] += this->actualMagSize;
		
		// dwa typy prze�adowa� czyli gdy starcza nam na uzupe�nienie ca�ego magazynku 
		// oraz na uzupe�nienie resztek kt�re nam zosta�y 
		int costOfWholeMag = this->origMagSize;
		int costOfAllAmmoLeft = Ammunitions[this->getType()];

		int decision = std::min(costOfWholeMag, costOfAllAmmoLeft);

		this->actualMagSize = decision;
		Ammunitions[this->getType()] -= decision;

		this->isReloading = false;

		this->timeElpasedForReloading.restart();
		this->delayBtwAutoReloading.restart();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
projectile* rangeWeapon::manageAddingSpecialBullets(playerParameters playerParams, int id)
{
	if (hasFlexibleBullets)
	{
		flexibleBullet *Projectile = new flexibleBullet;
		Projectile->create(playerParams.position, (playerParams.angle), this, id);
		return Projectile;
	}
	else if (hasSmartBulletsBoolean)
	{
		smartBullet *Projectile = new smartBullet;
		Projectile->create(playerParams.position, playerParams.angle, this, id);

		return Projectile;
	}
	else
	{
		projectile *Projectile = new projectile;
		Projectile->create(playerParams.position, (playerParams.angle), this, id);
		return Projectile;
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::tryReloading(std::map<typeOfWeapon, int>& Ammunitions)
{
	if (Ammunitions[this->EffectWeaponCauses.type] >= this->costOfBullet * std::pow(this->howManyProjItShoot,negate(this->packedBullets)))
	{
		this->timeElpasedForReloading.restart();
		this->isReloading = true;
	}
	else
	{
		this->isReloading = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::clear()
{
	// zwyk�y nudny pistolet z magazynkiem 12 kulek

	ID = 0;
	minDamage = 0;
	maxDamage = 0;
	bulletSize = 0;
	recoil = 0;
	firerate = 600;
	minSpeedOfBullet = 0;
	maxSpeedOfBullet = 0;
	howManyProjItShoot = 1;
	isAutomatic = 1;
	origMagSize = 12;
	actualMagSize = 12;
	costOfBullet = 1;
	reloadingTime = 1;
	packedBullets	= false;
	SAFAYCCLock		= false;
	isRecharger		= false;
	SAFAYCC			= false;



	EffectWeaponCauses.blank();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::create(weaponParameters params)
{
	ID							= params.ID;
	maxDamage					= params.maxDamage;
	minDamage					= params.minDamage;
	bulletSize					= params.bulletSize;
	recoil						= params.recoil;
	firerate					= params.firerate;
	minSpeedOfBullet			= params.minSpeedOfBullet;
	maxSpeedOfBullet			= params.maxSpeedOfBullet;
	EffectWeaponCauses			= params.weaponEffects;
	origMagSize					= params.origMagSize;
	actualMagSize				= params.actualMagSize;
	howManyProjItShoot			= params.howManyProjItShoot;
	this->costOfBullet			= params.costOfBullet;
	this->packedBullets			= params.packedBullets;

	initializeEffects();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::manageShooting(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile*>& ProjectilesArray, playerParameters playerParams)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&																		// sprawdzamy czy naci�ni�to LPM
		this->timeElapsedFirerate.getElapsedTime().asSeconds() > 60. / this->firerate &&									// sprawdzamy czy czas jest odpowiedni 
		this->actualMagSize >= std::pow(this->howManyProjItShoot, negate(this->packedBullets)) * this->costOfBullet)		// sprawdzamy czy magazynek zawiera odpowiedni� ilo�� kulek
	{
		for (int i = 0; i < this->howManyProjItShoot; ++i)
		{
			projectile* Proj = manageAddingSpecialBullets(playerParams, i);
			managePassingFunctionsToBullet(Proj);

			ProjectilesArray.push_back(Proj);
		}

		this->actualMagSize -= std::pow(this->howManyProjItShoot, negate(this->packedBullets)) * this->costOfBullet;

		timeElapsedFirerate.restart();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::managePassingFunctionsToBullet(projectile *proj)
{
	// (!) REFAKTORYZACJA 
	// mam 100% pewno�ci �e mo�na to zrobi� lepiej 
	// mniej linijek kodu i 
	if (this->getType() == typeOfWeapon::BULLETTYPE)
	{
		// wprowd�my troch� chaosu do funkcji				magic Number : chcemy by najmniejsza warto�� by�a 1-nk�
		//															||									   ||
		//														    \/									   \/
		double step = cast(double,(rand() % leadRangeOfRandomness + 1)) / (rand() % leadRangeOfRandomness + 1);
		proj->setFunctionPtr(projectileFunctions::leadAnimation,step);
	}
	else if (this->getType() == typeOfWeapon::PLASMA)
	{
		double step = cast(double, rand() % plasmaRangeOfRandomness + 1) / (rand() % plasmaRangeOfRandomness + 1);
		int anim = rand() % projectileFunctions::howManyPlasmaAnimations;
		if (anim == 0)
		{
			proj->setFunctionPtr(projectileFunctions::plasmaAnimation1, step);
		}
		else if (anim == 1)
		{
			proj->setFunctionPtr(projectileFunctions::plasmaAnimation2, step);
		}
		else if (anim == 2)
		{
			proj->setFunctionPtr(projectileFunctions::plasmaAnimation3, step);
		}
		else if (anim == 3)
		{
			proj->setFunctionPtr(projectileFunctions::plasmaAnimation4, step);
		}
		else if (anim == 4)
		{
			proj->setFunctionPtr(projectileFunctions::plasmaAnimation5, step);
		}
	}
	else if (this->getType() == typeOfWeapon::ENERGETIC)
	{
		double step = cast(double, rand() % energeticRangeOfRandomness + 1) / (rand() % energeticRangeOfRandomness + 1);
		int anim = rand() % projectileFunctions::howManyEnergeticAnimations;
		if (anim == 0)
		{
			proj->setFunctionPtr(projectileFunctions::energeticAnimation1, step);
		}
		else if (anim == 1)
		{
			proj->setFunctionPtr(projectileFunctions::energeticAnimation2, step);
		}
		else if (anim == 2)
		{
			proj->setFunctionPtr(projectileFunctions::energeticAnimation3, step);
		}
		else if (anim == 3)
		{
			proj->setFunctionPtr(projectileFunctions::energeticAnimation4, step);
		}
		else if (anim == 4)
		{
			proj->setFunctionPtr(projectileFunctions::energeticAnimation5, step);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool rangeWeapon::hasSmartBullets()
{
	return hasSmartBulletsBoolean;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
int rangeWeapon::howManyBulletsLeftInMag()
{
	return this->actualMagSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
int rangeWeapon::getMaxMagSize()
{
	return this->origMagSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
typeOfWeapon rangeWeapon::getType()
{
	return this->EffectWeaponCauses.type;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////