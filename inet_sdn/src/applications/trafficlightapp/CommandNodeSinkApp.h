

#ifndef COMMANDNODESINKAPP_H
#define COMMANDNODESINKAPP_H

#include "INETDefs.h"

#include "ApplicationBase.h"
#include "UDPSocket.h"

using namespace std;

class CommandNodeSinkApp : public ApplicationBase
{
  protected:
    UDPSocket socket;
    int numReceived;
    static simsignal_t rcvdPkSignal;

  protected:
    virtual void processPacket(cPacket *msg);

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

