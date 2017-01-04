/**
 * @file	Destroy.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Destroy.h"
#include "LocalFilter.h"


Destroy::Destroy(cModule* targetNode) : ActionBase(action_t::DESTROY)
{	
	this->targetNode = targetNode;
}


Destroy::~Destroy()
{	
}


void Destroy::execute () const
{
	int nodeID = targetNode->getId();
	LocalFilter* localFilter = check_and_cast<LocalFilter*> (targetNode->getSubmodule("localFilter"));
	localFilter->isDestroyed = true;
}
