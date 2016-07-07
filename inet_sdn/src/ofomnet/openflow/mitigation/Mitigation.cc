#include <omnetpp.h>
#include "openflow.h"
#include "IPv4Datagram.h"
#include "AttackDetection.h"
#include "OFA_controller.h"
#include "OF_Wrapper.h"

using namespace std;

Define_Module(Mitigation);

Mitigation::Mitigation() {}

Mitigation::~Mitigation() {}


void Mitigation::initialize() {
	mitigationSignal = registerSignal("Mitigation");
}

void Mitigation::handleMessage(cMessage *msg)
{
    opp_error("This module doesn't process messages");
}

void Mitigation::mitigate(IPv4Address victim, IPv4Address source, int inport) {
	ongoingMitigations.insert(pair<IPv4Address, IPv4Address>(source, victim));

	OF_Wrapper *wrapper = new OF_Wrapper();
	//prepare match rule
	oxm_basic_match *match = new oxm_basic_match();
	match->wildcards = OFPFW_NW_PROTO;
	match->OFB_IPV4_DST = victim;
	match->OFB_IPV4_SRC = source;
	match->OFB_ETH_TYPE = 2048;
	match->OFB_IN_PORT = inport;
	wrapper->match = match;
	EV << "[Mitigation]: Mitigation has been enabled at " << simTime() << " for dstIP: " << victim << " and srcIP: " << source << endl;
	emit(mitigationSignal, wrapper);
}

bool Mitigation::isOngoing(IPv4Address adr) {
	map<IPv4Address, IPv4Address>::iterator i;
	i = ongoingMitigations.find(adr);
	if (i != ongoingMitigations.end())
		return true;
	return false;
}

