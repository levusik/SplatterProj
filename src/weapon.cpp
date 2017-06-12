#include "entity.hpp"
#include <iostream>


//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::setEffects(effects efcsCausedByWpn)
{
	this->EffectWeaponCauses = efcsCausedByWpn;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::takeAction(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile>& ProjectilesArray, playerParameters PlayerParams)
{


	// je¿eli nie prze³adowujemy to strzelamy
	if (!this->isReloading)
	{

		//  sprawdzamy czy klikniêto w myszkê, jest wystarczaj¹co du¿o kulek (sanity check) i korzystamy ze wzoru na szybkostrzelnoœæ broni
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->actualMagSize >= this->howManyProjItShoot * this->costOfBullet)
		{
			if (this->isAutomatic && this->timeElapsedFirerate.getElapsedTime().asSeconds() > 60 / this->firerate)
			{
				this->manageShooting(Ammunitions, ProjectilesArray, PlayerParams);
				this->actualMagSize -= this->howManyProjItShoot * this->costOfBullet;
				this->timeElapsedFirerate.restart();
			}
			if (this->SAFAYCC && !this->SAFAYCCLock)
			{
				this->manageShooting(Ammunitions, ProjectilesArray, PlayerParams);
				this->actualMagSize -= this->howManyProjItShoot * this->costOfBullet;
				this->SAFAYCCLock = true;
			}

		}



		// sprawdzamy czy nie trzeba przypadkiem prze³adowaæ
		//														je¿eli magazynek ma niewystarczaj¹c¹ iloœæ kulek
		//														to próbujemy prze³adowaæ co 0.1s 
		if (this->actualMagSize < this->howManyProjItShoot * this->costOfBullet && this->delayBtwReloading.getElapsedTime().asSeconds() > 0.1f && !isReloading)
		{
			// próbujemy prze³adowaæ
			tryReloading(Ammunitions);
		}
	}
	// w przeciwnym wypadku czekamy
	else
	{
		// wyczekujemy moment prze³adowania okreœlony w pliku po czym uzupe³niamy kulki
		if (this->timeElpasedForReloading.getElapsedTime().asSeconds() > this->reloadingTime)
		{

			int subtract = std::min(this->origMagSize * this->costOfBullet * this->howManyProjItShoot, Ammunitions[this->getType()]/ (this->costOfBullet * this->howManyProjItShoot));
			std::cout << this->origMagSize * this->costOfBullet * this->howManyProjItShoot << " " << Ammunitions[this->getType()] / (this->costOfBullet * this->howManyProjItShoot) << "\n";

			if (!DEBUG)
			{
				std::cout << "skonczono przeladowanie ! : magazynek uzupelniono o " << subtract << " kulek.\n";
			}

			this->actualMagSize = subtract;
			Ammunitions[this->EffectWeaponCauses.type] -= subtract;
			this->isReloading = false;
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::tryReloading(std::map<typeOfWeapon, int>& Ammunitions)
{
	if (Ammunitions[this->EffectWeaponCauses.type] >= this->costOfBullet * this->howManyProjItShoot)
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
rangeWeapon::rangeWeapon()
{
	this->clear();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::clear()
{
	// zwyk³y nudny pistolet z magazynkiem 12 kulek

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
	this->SAFAYCC = 0;
	this->origMagSize = 12;
	this->actualMagSize = 12;
	this->costOfBullet = 1;
	this->reloadingTime = 1;
	this->SAFAYCCLock = 0;
	EffectWeaponCauses.blank();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::create(double minDamage, double maxDamage, double bulletSize, double recoil, double firerate, double minSpeedOfBullet, double maxSpeedOfBullet)
{
	this->minDamage = minDamage;
	this->maxDamage = maxDamage;
	this->bulletSize = bulletSize;
	this->recoil = recoil;
	this->firerate = firerate;
	this->minSpeedOfBullet = minSpeedOfBullet;
	this->maxSpeedOfBullet = maxSpeedOfBullet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::create(weaponParameters params)
{
	ID = params.ID;
	maxDamage = params.maxDamage;
	minDamage = params.minDamage;
	bulletSize = params.bulletSize;
	recoil = params.recoil;
	firerate = params.firerate;
	minSpeedOfBullet = params.minSpeedOfBullet;
	maxSpeedOfBullet = params.maxSpeedOfBullet;
	EffectWeaponCauses = params.EffectWeaponCauses;
	origMagSize = params.origMagSize;
	actualMagSize = params.actualMagSize;
	howManyProjItShoot = params.howManyProjItShoot;
	isAutomatic = params.isAutomatic;
	SAFAYCC = params.SAFAYCC;	// Shoot As Fast As You Can Click ( jakieœ rewolwery czy inne cuda)
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void rangeWeapon::manageShooting(std::map<typeOfWeapon, int>& Ammunitions, std::vector<projectile>& ProjectilesArray, playerParameters playerParams)
{

	// obs³uga specjalnych kulek
	switch (this->EffectWeaponCauses.type)
	{
	default:
		break;
	}

	std::cout << playerParams.angle << "\n";

	projectile Bullet;
	Bullet.create(playerParams.position, (playerParams.angle), this);
	ProjectilesArray.push_back(Bullet);

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
