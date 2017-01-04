/**
 * @file AttackEntry.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief AttackEntry class represents an attack entry object.
 */


#ifndef ATTACKENTRY_H
#define ATTACKENTRY_H

#include <iostream>
#include <omnetpp.h>

#include "AttackBase.h"


using namespace std;


class AttackEntry{

	private:
        // either occurrence time for physical attacks or first occurrence time for conditional or unconditional attacks
        SimTime occurrenceTime;
        // pointer-to attack
        AttackBase* attack;

	public:
        /** @brief Constructor */
        AttackEntry(AttackBase* attack, SimTime occurrenceTime);
            
        /** @brief Destructor. */
        virtual ~AttackEntry();

        /** @brief Get the occurrence time of the attack */
        SimTime getOccurrenceTime() const;
        
        /** @brief Get the pointer to the attack */
        AttackBase* getAttack() const;

};

#endif
