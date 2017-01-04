#ifndef RTUAPP_H
#define RTUAPP_H

#include "omnetpp.h"
#include "INETDefs.h"
#include "TCPSocket.h"
#include "Report.h"

using namespace std;

class RTUApp : public cSimpleModule, public cListener
{
	private:
	    enum SelfMsgKinds { CONNECT = 1, SEND };
        cMessage *timeoutMsg;       
        cMessage *reportMsg; 
        TCPSocket socket;
        Report *report;
        
        int interval;
        double startTime;
        // statistics
        int numSent;
        int numReceived;
        
        static simsignal_t rcvdPkSignal;
        static simsignal_t sentPkSignal;

    private:
        void connect();
        void startListening();
        void displayGUI();    
        
        void sendReportToDSO();   
                 
  	public:
  	    RTUApp();
  	    ~RTUApp();
    	virtual void initialize(int stage);
    	virtual void handleMessage(cMessage *msg);
    	virtual void finish();
 
};

#endif
