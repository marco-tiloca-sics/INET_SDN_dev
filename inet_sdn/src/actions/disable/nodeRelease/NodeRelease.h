/**
 * @file	NodeRelease.h
 * @author	Alexandra Stagkopoulou <alexandra.stagkopoulou@sics.se>
 * @brief	Clean up before node's disappearance. 
 			Release all the active listeners of the target node (module & channel).
 			Avoid unwanted warning messages.
 */


#ifndef NODERELEASE_H
#define NODERELEASE_H


#include <omnetpp.h>

using namespace std;

class NodeRelease {
	private:
		void unsubscribeModuleListeners (cModule* targetNode);
        void unsubscribeChannelListeners (cModule* targetNode);
        void unsubscribeChannelListener(cChannel *c);
	public:
		NodeRelease();
		virtual ~NodeRelease();
		
        void unsubscribeAndDeleteListeners(cModule* targetNode);

};

#endif
