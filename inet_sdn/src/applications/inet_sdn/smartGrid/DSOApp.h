#ifndef DSOAPP_H
#define DSOAPP_H

#include "omnetpp.h"
#include "INETDefs.h"
#include "TCPSocket.h"
#include "Report.h"

using namespace std;

class DSOApp : public cSimpleModule, public cListener
{
	private:
        cMessage *timeoutMsg;        
        TCPSocket socket;
        Report *report;
        

        
        int numReceived;       
        static simsignal_t rcvdPkSignal;
        
        void startListening();
        void displayGUI();
  	public:
  	    DSOApp();
  	    ~DSOApp();
  	    
    	virtual void initialize(int stage);
    	virtual void handleMessage(cMessage *msg);
    	virtual void finish();
 
};

#endif
