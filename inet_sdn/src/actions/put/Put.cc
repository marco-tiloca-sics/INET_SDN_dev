/**
 * @file	Put.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Put.h"


Put::Put(const direction_t direction, const double forwardingDelay, const bool isStatUpdated = false) : ActionBase(action_t::PUT)
{
	this->direction = direction;
	this->forwardingDelay = forwardingDelay;
	this->isStatUpdated = isStatUpdated;	
	involvedLayer = NONE_LAYER;	
}


Put::~Put()
{  
}


void Put::addRecipientNode(const int recipientNodeId)
{
	recipientNodes.push_back(recipientNodeId);	
}


double Put::getForwardingDelay() const
{
	return forwardingDelay;
}


cMessage* Put::execute(cMessage* packetToPut) const
{
	PutMsg* putmsg = new PutMsg(packetToPut, direction, isStatUpdated);
	putmsg->setForwardingDelay(forwardingDelay);

	for (size_t i = 0; i < recipientNodes.size(); i++) {
		putmsg->addRecipientNode(recipientNodes[i]);
	}
	
	// <A.S>
    delete packetToPut;

    // return putmsg
	return (cMessage*) putmsg;	
}

