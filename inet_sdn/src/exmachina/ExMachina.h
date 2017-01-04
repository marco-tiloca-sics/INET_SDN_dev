/**
 * @file	ExMachina.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 *
 * @brief	Experimental module. Executes ony disable. To extend for all physical attacks.
 */


#ifndef EXMACHINA_H
#define EXMACHINA_H


#include <omnetpp.h>
#include <vector>
#include "AttackEntry.h"


using namespace std;


class ExMachina : public cSimpleModule{

	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage* msg);
		virtual void finishSpecific();
	
	
	public:
		ExMachina();
		virtual ~ExMachina();
		
        void scheduleDisableAttack(AttackEntry* attack);

};

#endif
