/**
 * @file Move.cc
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Move.h"
#include "MobilityBase.h"


Move::Move(cModule* targetNode, Coord targetPosition) : ActionBase(action_t::MOVE)
{
	this->targetPosition = targetPosition;
	this->targetNode = targetNode;
}


Move::~Move()
{
}


void Move::execute() const
{	
	cModule* mobilityGenericSubmodule = targetNode->getSubmodule("mobility");

    // the node hasn't the mobility submodule
	if (mobilityGenericSubmodule == nullptr) {
		if(ev.isGUI()) {
			targetNode->getDisplayString().setTagArg("p", 0, (long)targetPosition.x);
			targetNode->getDisplayString().setTagArg("p", 1, (long)targetPosition.y);
		}
	}
	
	else {	
		MobilityBase* mobilitySubmodule = check_and_cast <MobilityBase*> (mobilityGenericSubmodule);
		// move the node to target position, translate its constraint area and update the visual representation
		mobilitySubmodule->transferToTargetPosition(targetPosition);
	}
}
