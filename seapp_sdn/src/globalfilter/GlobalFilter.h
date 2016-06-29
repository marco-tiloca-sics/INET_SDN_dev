/**
 * @file	GlobalFilter.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#ifndef GLOBALFILTER_H
#define GLOBALFILTER_H


#include <omnetpp.h>
#include <vector>

#include "AttackEntry.h"

using namespace std;


class GlobalFilter : public cSimpleModule {

	private:
		vector<AttackEntry*> unconditionalAttacks;
		vector<cMessage*> putMessages;
		vector<cMessage*> attackSelfMsgs;

	private:	
		void initializeAttacks();

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
