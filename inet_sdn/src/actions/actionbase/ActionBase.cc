/**
 * @file	ActionBase.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @version	1.0
 * @date	2015 jul 14
 */


#include "ActionBase.h"

#include <map>
#include <stdexcept>


const map<action_t,string> actionTypeMap = {
	{action_t::DESTROY,"Destroy"},
    {action_t::DISABLE,"Disable"},
	{action_t::MOVE,"Move"},
	{action_t::DROP,"Drop"},
	{action_t::CHANGE,"Change"},
	{action_t::RETRIEVE,"Retrieve"},
	{action_t::CLONE,"Clone"},
	{action_t::CREATE,"Create"},
	{action_t::SEND,"Send"},
	{action_t::PUT,"Put"},
	{action_t::EXPRESSION,"Expression"}
	// actually not supported by SEA++
	//{action_t::FAKEREAD,"Fakeread"},
};



string to_string(const action_t actionType) {
    switch (actionType) {
        case action_t::DESTROY: {return "Destroy";}
        case action_t::DISABLE: {return "Disable";}
        case action_t::MOVE: {return "Move";}
        case action_t::DROP: {return "Drop";}
        case action_t::CHANGE: {return "Change";}
        case action_t::RETRIEVE: {return "Retrieve";}
        case action_t::CLONE: {return "Clone";}
        case action_t::CREATE: {return "Create";}
        case action_t::SEND: {return "Send";}
        case action_t::PUT: {return "Put";}
        case action_t::EXPRESSION: {return "Expression";}
    }
}

/*
string to_string(const action_t actionType) {

	string mapped;
	
	try {
		mapped.assign( actionTypeMap.at(actionType) );
	}
	catch (const out_of_range& e) {
		opp_error("Out of range exception converting action_t to string");
	}
	
	return mapped;
	
}
*/

action_t to_action_t(const string actionType){
    if (actionType == "Destroy") {return action_t::DESTROY;}
    if (actionType == "Disable") {return action_t::DISABLE;}
    if (actionType == "Move") {return action_t::MOVE;}
    if (actionType == "Drop") {return action_t::DROP;}
    if (actionType == "Change") {return action_t::CHANGE;}
    if (actionType == "Retrieve") {return action_t::RETRIEVE;}
    if (actionType == "Clone") {return action_t::CLONE;}
    if (actionType == "Create") {return action_t::CREATE;}
    if (actionType == "Send") {return action_t::SEND;}
    if (actionType == "Put") {return action_t::PUT;}
    if (actionType == "Expression") {return action_t::EXPRESSION;}
    EV << "Error convertin string" + actionType + " to action_t";
}

/*
action_t to_action_t(const string actionType){

	action_t key;
		
	try {
		for (map<action_t,string>::const_iterator iter = actionTypeMap.begin(); iter != actionTypeMap.end(); ++iter) {
			if (iter->second == actionType) {
				key = iter->first;
				break;
			}
		}
		throw invalid_argument("Invalid argument exception converting " + actionType + " string to action_t");
	}
	catch (const invalid_argument& e) {
		opp_error(e.what());
	}
	
	return key;

}
*/


ActionBase::ActionBase (const action_t actionType) {
	
	this->actionType = actionType;
	
}


ActionBase::~ActionBase () {

}		


action_t ActionBase::getActionType () const {

	return actionType;

}


int ActionBase::getInvolvedLayer () const {

	return involvedLayer;

}


void ActionBase::setPacketName (const string packetName) {

	this->packetName.assign(packetName);

}


string ActionBase::getPacketName () const {

	return packetName;

}



