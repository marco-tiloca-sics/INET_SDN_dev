/**
 * @file PutMessages.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief PutMessages provides the messages used to perform the Put action.
 * @details During conditional and unconditional attacks, the communication process 
 *			between the global filter and the local filters is based on PutMessages messages, i.e. PutMsg and PutReq.
 */


#ifndef PUTMESSAGES_H
#define PUTMESSAGES_H


#include "omnetpp.h"


using namespace std;


enum class put_t {
	MSG = 0,
	REQ
};


enum class direction_t {
	RX = 0,
	TX,
};


class PutMsg : public cMessage {
	
	private:
		// a copy of the message to deliver
		cMessage* msg;
		// recipient nodes
		vector<int> recipientNodes;
		// packet direction (i.e. RX or TX)
		direction_t direction;
		// statistics update (RX mode only)
		bool isStatUpdated;
		// forwarding delay in seconds
		double forwardingDelay;

	public: 
		/**
		 * @brief	constructor
		 */
		PutMsg(cMessage* msg, const direction_t direction, const bool isStatUpdated);

		/**
		 * @brief	destructor
		 */
		virtual ~PutMsg();

		/**
		 * @brief setter/getter methods
		 */
		void setMsg(cMessage* msg);
		cMessage* getMsg() const;
		
		void setDirection(const direction_t direction);
		direction_t getDirection() const;
		
		void addRecipientNode(const int recipientNodeId);
		vector<int> getRecipientNodes() const;
		
		void setStatUpdated(const bool isStatUpdated);
		bool getStatUpdated() const;
		
		void setForwardingDelay(const double forwardingDelay);
		double getForwardingDelay() const;
		
};


class PutReq : public ::cMessage {
	
	private:
		// a copy of the message to deliver
		cMessage* msg;
		// packet direction (i.e. RX or TX)
		direction_t direction;
		// statistics update (RX mode only)
		bool isStatUpdated;

	public: 
		/**
		 * @brief	constructor
		 */
		PutReq(cMessage* msg, const direction_t direction, const bool isStatUpdated);

		/**
		 * @brief	destructor
		 */
		virtual ~PutReq();

		/**
		 * @brief setter/getter methods
		 */
		void setMsg(cMessage* msg);
		cMessage* getMsg() const;
		
		void setDirection(const direction_t direction);
		direction_t getDirection() const;
			
		void setStatUpdated(const bool isStatUpdated);
		bool getStatUpdated() const;
	
};


bool isPutMsg(cMessage* msg);
bool isPutReq(cMessage* msg);

#endif
