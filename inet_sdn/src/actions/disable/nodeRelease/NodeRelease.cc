/**
 * @file	NodeRelease.cc
 * @author	Alexandra Stagkopoulou <alexandra.stagkopoulou@sics.se>
 */

#include <omnetpp.h>
#include <vector>
#include "NodeRelease.h"


NodeRelease::NodeRelease () {}

NodeRelease::~NodeRelease () {}


void NodeRelease::unsubscribeAndDeleteListeners (cModule* targetNode) {
    unsubscribeModuleListeners(targetNode);
    unsubscribeChannelListeners(targetNode);
}

void NodeRelease::unsubscribeModuleListeners (cModule* targetNode) {
    vector<simsignal_t> signals = targetNode->getLocalListenedSignals();

    for (unsigned int i = 0; i < signals.size(); i++) {
        simsignal_t signalID = signals[i];
        vector<cIListener*> listeners = targetNode->getLocalSignalListeners(signalID);
        for (unsigned int j=0; j< listeners.size(); j++)
            targetNode->unsubscribe(signals[i], listeners[j]);
    }
}

void NodeRelease::unsubscribeChannelListeners (cModule* targetNode) {
    //channel listeners
    vector<const char *> gateName = targetNode->getGateNames();

    for (unsigned int i=0; i < gateName.size(); i++) { //all present gates
        int gateSize = targetNode->gateSize(gateName[i]);
        if(gateSize != 0) {
            for(unsigned int j = 0; j < gateSize; j++) {
                if (targetNode->gateType(gateName[i]) == cGate::INOUT) {
                    if (targetNode->isGateVector(gateName[i])) { 
                        cGate *g = targetNode->gateHalf(gateName[i], cGate::OUTPUT, j);
                        cChannel *c = g->getTransmissionChannel();
                        unsubscribeChannelListener(c);
                    }
                }         
            }
        }
    }  
}

void NodeRelease::unsubscribeChannelListener(cChannel *c) {
    std::vector<simsignal_t> signals = c->getLocalListenedSignals();

    for (unsigned int i = 0; i < signals.size(); i++) {
        simsignal_t signalID = signals[i];
        std::vector<cIListener*> listeners = c->getLocalSignalListeners(signalID);
        
        for (unsigned int j=0; j < listeners.size(); j++)
            c->unsubscribe(signals[i], listeners[j]);
    }    
}