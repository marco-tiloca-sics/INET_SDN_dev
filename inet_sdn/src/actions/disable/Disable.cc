/**
 * @file	Disable.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Disable.h"
#include "NodeRelease.h"

Disable::Disable (cModule* targetNode):ActionBase(action_t::DISABLE) {
	this->targetNode = targetNode;
}


Disable::~Disable () {}


void Disable::execute () const {
    
    EV << "[Disable::execute()] invoked" << endl;
    targetNode->callFinish();
    // <A.S> : Clean up before node's disappearance. Release all listeners.
    NodeRelease *request = new NodeRelease();
    request->unsubscribeAndDeleteListeners(targetNode);
    targetNode->deleteModule();
    
}
