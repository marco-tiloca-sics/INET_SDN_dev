/**
 * @file	ActionBase.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "ActionBase.h"


ActionBase::ActionBase(const action_t actionType)
{	
	this->actionType = actionType;	
}


ActionBase::~ActionBase()
{
}		


action_t ActionBase::getActionType() const
{
	return actionType;
}


int ActionBase::getInvolvedLayer() const
{
	return involvedLayer;
}


void ActionBase::setPacketName(const string packetName)
{
	this->packetName.assign(packetName);
}


string ActionBase::getPacketName() const
{
	return packetName;
}


string to_string(const action_t type)
{
	switch(type){
	
		case action_t::DESTROY:{
			return "Destroy";
		}
		
		case action_t::MOVE:{
			return "Move";
		}
		
		case action_t::FAKEREAD:{
			return "Fakeread";
		}

		case action_t::CHANGE:{
			return "Change";
		}

		case action_t::RETRIEVE:{
			return "Retrieve";
		}
		
		case action_t::CLONE:{
			return "Clone";
		}
		
		case action_t::CREATE:{
			return "Create";
		}
		
		case action_t::DROP:{
			return "Drop";
		}
		
		case action_t::PUT:{
			return "Put";
		}
		
		case action_t::SEND:{
			return "Send";
		}
		
		case action_t::EXPRESSION:{
			return "Expression";
		}
	
		default:{
			opp_error("[string to_string(const action_t)] Error, type not recognized.");
		}

	}
}


action_t to_action_t(const string type)
{
	if (type == "Destroy") {
		return action_t::DESTROY;
	}
	
	if (type == "Move") {
		return action_t::MOVE;
	}

	if (type == "Fakeread") {
		return action_t::FAKEREAD;
	}

	if (type == "Change") {
		return action_t::CHANGE;
	}

	if (type == "Retrieve") {
		return action_t::RETRIEVE;
	}

	if (type == "Clone") {
		return action_t::CLONE;
	}

	if (type == "Create") {
		return action_t::CREATE;
	}

	if (type == "Drop") {
		return action_t::DROP;
	}

	if (type == "Put") {
		return action_t::PUT;
	}

	if (type == "Send") {
		return action_t::SEND;
	}

	if (type == "Expression") {
		return action_t::EXPRESSION;
	}
	
	opp_error("[action_t to_action_t(const string)] Error, type not recognized");
}
