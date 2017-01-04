/**
 * @file	ConditionalAttack.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	Conditional class represents a conditional attack, which is carried out by LocalFilter sub-modules.
 */
 

#ifndef CONDITIONALATTACK_H
#define CONDITIONALATTACK_H


#include "PacketFilter.h"
#include "AttackBase.h"


using namespace std;



class ConditionalAttack : public AttackBase {

	private:
		// pointer-to the packet-filter
        PacketFilter* packetFilter;
		//minimumInvolvedLayer is the minimum TCP/IP layer involved in the conditional attack 
		int minimumInvolvedLayer;
		
		
			
	public:
		/** 
		 * @brief Constructor.
		 */
		ConditionalAttack();
		
		/** 
		 * @brief Destructor.
		 */
		virtual ~ConditionalAttack();
		
		/**
         * @brief Adds (appends) an action to the conditional attack.
         * @param action Is the pointer-to the action to add to the 
         *        conditional attack.
         */
		virtual void addAction(ActionBase* action);

		/**
         * @brief Setter method, sets the packet-filter.
         * @param monolithicPacketFilter Is the packet-filter to add 
         *        to the conditional attack.
         */
		void setPacketFilter(string monolithicPacketFilter);

		/**
         * @brief Checks if the the current packet matches the 
         *        packet-filter.
         * @param packet Is the pointer-to the current packet.
         * @return Returns true if the current packet matches the 
         *         packet-filter, false otherwise.
         */
		bool matchPacketFilter(cMessage* packet) const;

		// TODO redesign signature
        /**
         * @brief Executes (all the actions composing) the conditional 
         *        attack.
         * @param ...
         */
		void execute(cMessage** packet, vector<cMessage*> &generatedPackets, vector<double> &delays, double &delay);

};

#endif
