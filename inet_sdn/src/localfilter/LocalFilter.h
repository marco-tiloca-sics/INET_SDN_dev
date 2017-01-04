/**
 * @file	LocalFilter.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#ifndef LOCALFILTER_H
#define LOCALFILTER_H


#include <omnetpp.h>
#include <vector>
#include <map>

#include "ConditionalAttack.h"


using namespace std;


class LocalFilter : public cSimpleModule {

	friend class Destroy;
	friend class Create;

	private:
		enum class command_t {
			DISCARD = 0,
			SELFMESSAGE,
			GLOBALFILTER,
			CONDITIONAL,
			NOATTACK	
		};
		
        // node's status
		bool isDestroyed;
        // map input-output gates
		map<cGate*,cGate*> coupledGates;
		// conditional attack enabled
		vector<ConditionalAttack*> enabledConditionalAttacks;
        // injected bytes counter
		int injectedBytes;
		
		// <A.S>
		string networkAddr;
		string netmask;

	private:
		void forgeInterfaceTable();
		void initializeGates();
		void initializeAttacks();
		void forgeSendingData(cMessage* msg);
		command_t planOperation(cMessage* msg) const;
        // <A.S>
        void getNetworkParameters();

	protected:
		virtual void initialize(int stage);
		virtual void handleMessage(cMessage* msg);
		virtual int numInitStages() const { return 5;};
	
	public:
        /**
         * @brief Constructor.
         */
		LocalFilter();
		
        /**
         * @brief Destructor.
         */
        virtual ~LocalFilter();
		
};

#endif
