/**
 * @file	ExMachina.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "ExMachina.h"
#include "AttackBase.h"
#include "ActionBase.h"
#include "PhysicalAttack.h"

Define_Module(ExMachina);


void ExMachina::initialize () {

}


void ExMachina::handleMessage (cMessage* msg) {

	bool isSelfMessage = msg->isSelfMessage();
    
    // handles only self messages, i.e. scheduled disable attacks
	if (isSelfMessage) {
        PhysicalAttack* physicalAttack = (PhysicalAttack*) (msg->par("attack").pointerValue());
        physicalAttack->execute();
	}
	
	delete msg;

}


void ExMachina::finishSpecific () {

}


ExMachina::ExMachina () {	

}


ExMachina::~ExMachina () {

}


void ExMachina::scheduleDisableAttack (AttackEntry* attack) {

    Enter_Method("scheduleDisableAttack()");
    cMessage* selfMessage = new cMessage("Fire physical attack", (short) attack_t::PHYSICAL);
    selfMessage->addPar("attack");
    selfMessage->par("attack").setPointerValue(attack->getAttack());
    scheduleAt(attack->getOccurrenceTime(), selfMessage);
    
}



