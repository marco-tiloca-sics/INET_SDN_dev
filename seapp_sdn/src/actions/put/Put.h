/**
 * @file	Put.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	The Put class is used to deliver a packet to a set of nodes.
 * @details	The Put class is used to deliver a packet to a set of nodes. Three modes are available:
 *			 - TX 	the packet is put in the TX buffer of the specified layer
 *			 - RX & STATS 	the packet is put in the RX buffer of the specified layer as it passes through the channel
 *			 - RX & !STATS	the packet is put in the RX buffer of the specified layer as it doesn't pass through the channel
 * 
 */


#ifndef PUT_H
#define PUT_H


#include "ActionBase.h"
#include "PutMessages.h"

class Put : public ActionBase {

	private:
		vector<int> recipientNodes;
		direction_t direction;
		double forwardingDelay;
		bool isStatUpdated;

	public:
		/**
		 * @brief Constructor
		 */
		Put(const direction_t direction, const double forwardingDelay, const bool isStatUpdated);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Put();

		/**
		 * @brief add a recipient node
		 */
		void addRecipientNode(const int recipientNodeId);
		
		/**
		 * @brief get forwarding delay
		 */
		double getForwardingDelay() const;
		
		/**
		 * @brief execute the send action
		 */
		cMessage* execute(cMessage* packetToPut) const;
		
};

#endif
