
#ifndef SMARTMETER_H
#define SMARTMETER_H

#include "INETDefs.h"
#include "omnetpp.h"


using namespace std;

class SmartMeter : public cSimpleModule
{
    private:       
        //signal
        simsignal_t measurementDataSignal;
	private:
		void sendMeasurementData();
		void nextReading();

  	public:
    	virtual void initialize();
    	virtual void handleMessage(cMessage *msg);
    	virtual void finish();
 
};

#endif
