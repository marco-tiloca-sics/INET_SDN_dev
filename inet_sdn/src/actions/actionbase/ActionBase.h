/**
 * @file	ActionBase.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @version	1.0
 * @date	2015 jul 14
 *
 * @brief	ActionBase class provides the representation of a generic action that can be performed during an attack.
 *
 * @details	ActionBase class provides the type action_t, a scoped enum used to represent actions.
 
 */
 

#ifndef ACTIONBASE_H
#define ACTIONBASE_H


#include <omnetpp.h>
#include "INETDefs.h"
#include <string>
#include <cstdint>


#define NONE_LAYER 1000


using namespace std;


// action type
enum class action_t : uint8_t{
	DESTROY = 0,
	DISABLE,
    MOVE,
	DROP,
	CHANGE,
	RETRIEVE,
	CLONE,
	CREATE,
	SEND,
	PUT,
	EXPRESSION
};


// utility functions
string to_string(const action_t actionType);
action_t to_action_t(const string actionType);


class ActionBase {

	private:
		// action type
		action_t actionType;
		// name of the packet related to the action
		string packetName;
		
	protected:
		// layer involved in the action 
		int involvedLayer;
	
	protected:
		/**
		 * @brief	Constructor
		 * @param	actionType it is the type of the action
		 */
		ActionBase(const action_t actionType);
		
	public:
		/** 
		 * @brief	Destructor
		 */
		~ActionBase();

		/**
		 * @brief	Setter and getter methods
		 */
		action_t getActionType() const;
		virtual void setPacketName(const string packetName);
		virtual string getPacketName() const;
		virtual int getInvolvedLayer() const;

};


#endif
