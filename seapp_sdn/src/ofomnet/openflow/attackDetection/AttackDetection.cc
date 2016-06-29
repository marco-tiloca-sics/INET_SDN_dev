#include <omnetpp.h>

#include "openflow.h"
#include "IPv4Datagram.h"
#include "AttackDetection.h"
#include "Entropy.h"
#include "Mitigation.h"
#include "OFA_controller.h"

using namespace std;

Define_Module(AttackDetection);

AttackDetection::AttackDetection() {}

AttackDetection::~AttackDetection() {}


void AttackDetection::initialize() {
	cModule *ITModule = getParentModule()->getSubmodule("entropy");
    entropy = check_and_cast<Entropy *>(ITModule);
    
    cModule *ITModule2 = getParentModule()->getSubmodule("mitigation");
    mitigation = check_and_cast<Mitigation *>(ITModule2);
    
    cModule *ITModule3 = getParentModule()->getSubmodule("ofa_controller");
    controller = check_and_cast<OFA_controller *>(ITModule3);
    
    rcv_cnt_threshold = par("received_packet_threshold");
    sent_cnt_threshold = par("sent_packet_threshold");
}

void AttackDetection::handleMessage(cMessage *msg)
{
    opp_error("This module doesn't process messages");
}


/*
 * Extract info for the destination IPs and the respective counters.
 * Apply the entropy algorithm on them to identify a possible attack.
 */

//int = datapath ID, ofp_flow_stats * = of struct
void AttackDetection::dataAnalysis(std::map<int, ofp_flow_stats *> &stats) {
	std::map<IPv4Address, int> dataSetDst = collectInfo(stats, "dst");
	bool alert = false;	
	
	if (!dataSetDst.empty()) 
		alert = entropy->entropyCalc(dataSetDst);

	if (alert) {
		IPv4Address victim = identifyTarget(dataSetDst, "dst");
		if (!victim.isUnspecified()) {
			//identify source
			std::map<IPv4Address, int> dataSetSrc = collectInfo(stats, "src");
			IPv4Address source = identifyTarget(dataSetSrc, "src");
			if (!victim.isUnspecified()) {				
				//confirm flow match between source, victim
				if (existingFlow(stats, source, victim)) {
					//check if there is ongoing mitigation
					if (!mitigation->isOngoing(source)) {
						int inport = findInPort(stats, source);
						//start mitigation
						mitigation->mitigate(victim, source, inport);
						dataSetSrc.clear();
					}
				}
			}
		}
	}
	dataSetDst.clear();
	
}

/*create a dataset with all the dst/src addresses and the counters corresponding to them*/
std::map<IPv4Address, int> AttackDetection:: collectInfo(std::map<int, ofp_flow_stats *> &stats, string target) {
	std::map<IPv4Address, int> dataSet;
	std::map<IPv4Address, int>::iterator dataSetIt;
	IPv4Address addr;	
	
	//accumulate the IP counters
	std::map<int, ofp_flow_stats *>::iterator it = stats.begin();
	while (it!= stats.end()) {
		int datapath = it->first;
		int size = it->second[0].size;
		//condition1: Collect all IP traffic (regardless protocol)
		for (int i=0; i<size; i++) {
			int packet_type = it->second[i].match.OFB_ETH_TYPE;
			if (packet_type == 2048) { //IP Packet 
				if (target == "dst") 
					addr = it->second[i].match.OFB_IPV4_DST;
				else if(target == "src")
					addr = it->second[i].match.OFB_IPV4_SRC;
				else
					opp_error("[void AttackDetection::collectInfo(std::map<int, ofp_flow_stats *> &stats, sting target)] can't recognize the target, which is not supported");
			
				int counter = it->second[i].packet_count;
				dataSetIt = dataSet.find(addr);
				if(dataSetIt == dataSet.end() )
					dataSet.insert(std::make_pair(addr, counter));
				else
					dataSetIt->second += counter;
			}
		}
		it++;
	}
	return dataSet;
} 


//find the IP Destination address with the maximum counter number
IPv4Address AttackDetection::identifyTarget(std::map<IPv4Address, int> &data, string target) {
	int max = 0;
	IPv4Address address = IPv4Address();
	std::map<IPv4Address, int>::iterator i =  data.begin();
	
	double interval = controller->returnStatsInterval();
	
	while (i!= data.end()) {
		if (i->second >= max) {
			max = i->second;
			if(target == "dst") {	
				if (max > (rcv_cnt_threshold*interval)) {
					address = i->first;
				}
			}
			else if (target == "src") {
				if (max > (sent_cnt_threshold*interval)) {
					address = i->first;
				}
			}
		}
		i++;	
	}
	
	return address;
}


int AttackDetection::findInPort(std::map<int, ofp_flow_stats *> &stats, IPv4Address src) {
	int inport;
	std::map<int, ofp_flow_stats *>::iterator it = stats.begin();
	while (it!= stats.end()) {
		int size = it->second[0].size;
		for (int i=0; i<size; i++) {
			if(it->second[i].match.OFB_IPV4_SRC == src) 
				return it->second[i].match.OFB_IN_PORT;
		}
		it++;
	}
	return inport;
}

bool AttackDetection::existingFlow(std::map<int, ofp_flow_stats *> &stats, IPv4Address src, IPv4Address dst) {
	std::map<int, ofp_flow_stats *>::iterator it = stats.begin();
	while (it!= stats.end()) {
		int size = it->second[0].size;
		for (int i=0; i<size; i++) {
			if((it->second[i].match.OFB_IPV4_SRC == src) && (it->second[i].match.OFB_IPV4_DST == dst)) 
				return true;
		}
		it++;
	}
	return false;
}


