/**
 * @file	ActionBase.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	ActionBase class provides the representation of a generic action that can be performed during an attack.
 */
 

#ifndef ACTIONBASE_H
#define ACTIONBASE_H

#include "omnetpp.h"
#include "INETDefs.h"

#include <string>


#define NONE_LAYER 1000


using namespace std;


// type of the action
enum class action_t {
	DESTROY = 0,
	MOVE,
	DROP,
	CLONE,
	CHANGE,
	SEND,
	PUT,
	RETRIEVE,
	EXPRESSION,
	CREATE,	
	FAKEREAD
};


class ActionBase {

	private:
        // type of the action
		action_t actionType;
        // name of the target packet
		string packetName;
		
	protected:
        // TODO change in minimumInvolvedLayer
        // minimum layer involved in the action
		int involvedLayer;
	
	protected:
		/**
		 * @brief	Constructor
		 * @param	action_t type of the action
		 */
		ActionBase(const action_t actionType);
		
	public:
		/** 
         * @brief Destructor
         */
		~ActionBase();
		
		/** 
         * @brief Get the type of the action
         */
		action_t getActionType() const;
	
        /**
         * @brief Get the minimum involved layer
         */
		virtual int getInvolvedLayer() const;
		
        /**
         * @brief Set the name of the target packet
         */
		virtual void setPacketName(const string packetName);
        
        /**
         * @brief Get the name of the target packet
         */
		virtual string getPacketName() const;
};

/**
 * @brief utilities functions
 */
string to_string(const action_t type);
action_t to_action_t(const string type);

#endif
