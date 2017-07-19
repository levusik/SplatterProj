#pragma once
#include "enemy.hpp"

class flightyEnemy : public enemy
{
public:
	flightyEnemy()
	{
		groupAnimal = true;
	}
protected:

	virtual void IDLEState()						override;
	virtual void IDLESChangeToDiffState()			override;

	virtual void intriguedState()					override;
	virtual void intriguedSChangeToDiffState()		override;

	virtual void makingDecisionState()				override;
	virtual void makingDecisionChangeToDiffState()	override;

	virtual void fleeState()						override;
	virtual void fleeChangeToDiffState()			override;

	virtual void agressiveState()					override;
	virtual void agressiveChangeToDiffState()		override;

	virtual void chargeState()						override;
	virtual void chargeChangeToDiffState()			override;

	virtual void attackState()						override;
	virtual void attackChangeToDiffState()			override;

	virtual void specialAbility()					override;
	virtual void specialAbilityChangeToDiffState()	override;

	virtual bool isWorthAttacking()			override;

	virtual void reactToGettingHit()		override;
	virtual void gettingHitChangeStates()	override;

	virtual void playAttackAnim()			override;
	virtual void randomizeMovement()		override;
};