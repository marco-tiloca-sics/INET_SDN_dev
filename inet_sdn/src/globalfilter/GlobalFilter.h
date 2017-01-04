/**
 * @file	GlobalFilter.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#ifndef GLOBALFILTER_H
#define GLOBALFILTER_H


#include <omnetpp.h>
#include <vector>

#include "AttackEntry.h"
#include "PutMessages.h"

using namespace std;


class GlobalFilter : public cSimpleModule {

	private:
		vector<AttackEntry*> unconditionalAttacks;

	    // <A.S>
		string networkAddr;
		string netmask;

	private:	
		void initializeAttacks();
		// <A.S>
        void getNetworkParameters();

	protected:
		virtual void initialize();
		virtual void handleMessage(cMessage* msg);
		virtual void handlePutMessage(const cMessage* msg);
		virtual void finishSpecific();
	
	
	public:
		GlobalFilter();
		virtual ~GlobalFilter();
};

#endif
