/**
 * @file	Send.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Send.h"
#include "seapputils.h"


Send::Send(const double delay) : ActionBase(action_t::SEND)
{
	this->delay = delay;
	involvedLayer = NONE_LAYER;	
}


Send::~Send()
{
}


double Send::getSendDelay() const
{
	return delay;
}


// TODO remove the check on the parameter 'isToSend'
void Send::execute(cMessage* packetToSend) const
{
	bool hasParameter = false;
	hasParameter = packetToSend->hasPar("isToSend");

	if (!hasParameter) {
		string errorMsg;
		errorMsg.append("Send::execute can't find isToSend parameter");
		opp_error(errorMsg.c_str());
	}
	else {
		packetToSend->par("isToSend").setBoolValue(true);
	}
}
