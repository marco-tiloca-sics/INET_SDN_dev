
#ifndef TRAFFICLIGHTUPDATESTATUSAPP_H
#define TRAFFICLIGHTUPDATESTATUSAPP_H

#include "INETDefs.h"
#include "omnetpp.h"
#include <string>

using namespace std;

class TrafficLightUpdateStatusApp : public cSimpleModule
{

	private:
		double updatePeriod = 0.2;

	private:
		void setRed();
		void setYellow();
		void setGreen();
		void updateFile(double t, string color);
  	public:
    	virtual void initialize();
    	virtual void handleMessage(cMessage *msg);
    	virtual void finish();
 
};

#endif

