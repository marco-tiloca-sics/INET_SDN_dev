/**
 * @file Send.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief The Send class is used to:
 *			- send (with a specified delay) a created or cloned packet to the next layer in the tx/rx flow
 *			- send (with a specified delay) an intercepted packet to the next layer in the tx/rx flow
 */


#ifndef SEND_H
#define SEND_H


#include "ActionBase.h"


class Send : public ActionBase {

	private:
		double delay;

	public:
		/**
		 * @brief Constructor
		 */
		Send(const double delay);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Send();

		/**
		 * @brief returns the delay
		 */
		 double getSendDelay() const;
		
		/**
		 * @brief execute the send action
		 */
		void execute(cMessage* packetToSend) const;
		
};

#endif
