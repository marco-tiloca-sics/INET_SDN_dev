#ifndef DATACONCENTRATORAPP_H
#define DATACONCENTRATORAPP_H

#include "omnetpp.h"
#include "INETDefs.h"
#include "TCPSocket.h"
#include "Report.h"

using namespace std;

class DataConcentratorApp : public cSimpleModule, public cListener
{
    private:
        enum SelfMsgKinds { CONNECT = 1, SEND };
        Report *report;
        cListener *listener;
        int interval;
            
        simtime_t startTime;
        simtime_t stopTime;
        cMessage *timeoutMsg;
        cMessage *reportMsg;
        
        TCPSocket socket;
        
        // statistics
        int numSent;
        int numReceived;
        
        static simsignal_t rcvdPkSignal;
        static simsignal_t sentPkSignal;
	private:
		void sendMeasurementData();
		void sendReportToRTU();
		//TCP related functions
        void connect();
        void bind();		
		void displayGUI();
	protected:
	    void receiveSignal(cComponent *src, simsignal_t id, cObject *obj);

  	public:
  	    DataConcentratorApp();
  	    ~DataConcentratorApp();
    	virtual void initialize(int stage);
    	virtual void handleMessage(cMessage *msg);
    	virtual void finish();
 
};

#endif
