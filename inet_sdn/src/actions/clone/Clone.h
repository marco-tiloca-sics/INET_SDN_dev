/**
 * @file Clone.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief The Clone class creates a perfect copy of a given packet.
 */


#ifndef CLONE_H
#define CLONE_H


#include "ActionBase.h"


class Clone : public ActionBase {

	private:
		string clonePacketName;

	public:
		/**
		 * @brief Constructor
		 */
		Clone(const string clonePacketName, const string originalPacketName);
		
		/**
		 * @brief Destructor
		 */
		virtual ~Clone();

		/**
		 * @brief returns the name of the cloned packet
		 */
		string getClonePacketName() const;
		
		/**
		 * @brief execute the clone action
		 */
		void execute(cMessage** clonePacket, const cMessage* originalPacket) const;
		
};

#endif
