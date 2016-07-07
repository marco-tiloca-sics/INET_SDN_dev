/**
 * @file	PutMessage.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "PutMessages.h"
#include "seapputils.h"


PutMsg::PutMsg(cMessage* msg, const direction_t direction, const bool isStatUpdated) : cMessage("PutMsg", (short)put_t::MSG)
{
	this->msg = (cMessage*) hardCopy((cPacket*) msg);
	// reset isFiltered
	setParameterRecursively(this->msg, "isFiltered", false);
	// take the ownership
	take(this->msg);
	this->direction = direction;
	// enable statistics only in RX mode
	if (direction == direction_t::TX) {
		if(isStatUpdated){
			EV << "PutMessage::PutMessage can't update statistics in TX mode" << endl;
		}
		this->isStatUpdated = false;		
	}
	else{
		this->isStatUpdated = isStatUpdated;
	}
}


PutMsg::~PutMsg()
{
	// release the ownership
	drop(msg);
	delete msg; 
	
}


void PutMsg::setMsg(cMessage* msg)
{
	this->msg = msg;
}


cMessage* PutMsg::getMsg() const
{
	return msg;
}
		

void PutMsg::setDirection(const direction_t direction)
{
	this->direction = direction;
}

		
direction_t PutMsg::getDirection() const
{
	return direction;
}
		

void PutMsg::addRecipientNode(const int recipientNodeId)
{
	this->recipientNodes.push_back(recipientNodeId);
}


vector<int> PutMsg::getRecipientNodes() const
{
	return recipientNodes;
}
		

void PutMsg::setStatUpdated(const bool isStatUpdated)
{
	this->isStatUpdated = isStatUpdated;
}


bool PutMsg::getStatUpdated() const
{
	return isStatUpdated;
}
		

void PutMsg::setForwardingDelay(const double forwardingDelay)
{
	this->forwardingDelay = forwardingDelay;
}


double PutMsg::getForwardingDelay() const
{
	return forwardingDelay;
}


PutReq::PutReq(cMessage* msg, const direction_t direction, const bool isStatUpdated) : cMessage("PutReq", (short)put_t::REQ)
{
	this->msg = (cMessage*) hardCopy((cPacket*) msg);
	// reset isFiltered
	setParameterRecursively(this->msg, "isFiltered", false);
	// take the ownership
	take(this->msg);
	this->direction = direction;
	
	// enable statistics only in RX mode
	if (direction == direction_t::TX) {
		if(isStatUpdated){
			EV << "PutMessage::PutMessage can't update statistics in TX mode" << endl;
		}
		this->isStatUpdated = false;		
	}
	else{
		this->isStatUpdated = isStatUpdated;
	}

}


PutReq::~PutReq()
{
	// release the ownership
	drop(msg);
	delete msg;
}


void PutReq::setMsg(cMessage* msg)
{
	this->msg = msg;
}


cMessage* PutReq::getMsg() const
{
	return msg;
}
		

void PutReq::setDirection(const direction_t direction)
{
	this->direction = direction;
}

		
direction_t PutReq::getDirection() const
{
	return direction;
}
		

void PutReq::setStatUpdated(const bool isStatUpdated)
{
	this->isStatUpdated = isStatUpdated;
}


bool PutReq::getStatUpdated() const{
	return isStatUpdated;
}


bool isPutMsg (cMessage* msg){

	if (std::string(msg->getClassName()) == "PutMsg") {
		return true;
	}

	return false;
}


bool isPutReq(cMessage* msg)
{
	if (std::string(msg->getClassName()) == "PutReq") {
		return true;
	}

	return false;
}
