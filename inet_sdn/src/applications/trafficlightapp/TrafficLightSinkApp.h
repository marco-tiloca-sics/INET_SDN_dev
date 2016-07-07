

#ifndef TRAFFICLIGHTSINKAPP_H
#define TRAFFICLIGHTSINKAPP_H

#include "INETDefs.h"

#include "ApplicationBase.h"
#include "UDPSocket.h"
#include <string>

using namespace std;

class TrafficLightSinkApp : public ApplicationBase
{
	private:
		double updatePeriod = 0.2;

	protected:
		UDPSocket socket;
		int numReceived;
		static simsignal_t rcvdPkSignal;

	protected:
		virtual void processPacket(cPacket *msg);
		void updateFile(double t, string color);
		void setRed();
		void setYellow();
		void setGreen();

	protected:
		virtual int numInitStages() const { return 4; }
		virtual void initialize(int stage);
		virtual void handleMessageWhenUp(cMessage *msg);
		virtual void finish();

		virtual bool handleNodeStart(IDoneCallback *doneCallback);
		virtual bool handleNodeShutdown(IDoneCallback *doneCallback);
		virtual void handleNodeCrash();
};

#endif

