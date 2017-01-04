/**
 * @file	AttackEntry.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "AttackEntry.h"


AttackEntry::AttackEntry(AttackBase* attack, SimTime occurrenceTime)
{
	this->attack = attack;
	this->occurrenceTime = occurrenceTime;
}


AttackEntry::~AttackEntry()
{
	// TODO delete attack
    //delete attack;
}


SimTime AttackEntry::getOccurrenceTime() const
{
	return occurrenceTime;
}


AttackBase* AttackEntry::getAttack() const
{
	return attack;
}
