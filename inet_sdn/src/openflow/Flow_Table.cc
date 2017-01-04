#include <omnetpp.h>
#include <EtherFrame_m.h>
#include <MACAddress.h>
#include <ARPPacket_m.h>
#include <Flow_Table.h>
#include <list>
#include <stdlib.h>

char hexString[10];

using namespace std;

Define_Module(Flow_Table);

Flow_Table::Flow_Table() {}

void Flow_Table::initialize()
{
    size = 0;
    WATCH(size);
}

static int flow_fields_match(oxm_basic_match *m1, oxm_basic_match *m2, uint32_t w)
{	
	int returnVal;

	if ((w && OFPFW_IN_PORT) || (m1->OFB_IN_PORT == m2->OFB_IN_PORT)) { 
		//ARP Packet
		//if (m1->OFB_ETH_TYPE == 2054 ) { 
			return ( ((w & OFPFW_DL_TYPE) || (m1->OFB_ETH_TYPE == m2->OFB_ETH_TYPE ))
            		&& ((w & OFPFW_DL_SRC) || (!m1->OFB_ETH_SRC.compareTo(m2->OFB_ETH_SRC)))
           			&& ((w & OFPFW_DL_DST) || (!m1->OFB_ETH_DST.compareTo(m2->OFB_ETH_DST))) );
	/*	}
		//IP Packet
		else if (m1->OFB_ETH_TYPE == 2048) {
			//mitigation rule: mitigation rule is applied for all IP packets regardless the net protocol
			if (w & OFPFW_NW_PROTO) { 
		    	returnVal =  ( (m1->OFB_IPV4_SRC == m2->OFB_IPV4_SRC)
		       				&& (m1->OFB_IPV4_DST == m2->OFB_IPV4_DST));
		       	if (returnVal) 
		       		return returnVal;
		    } 
			//ICMP Packet
			if (m1->OFB_IP_PROTO == 1 ) {
				return ( ((w & OFPFW_DL_TYPE) || (m1->OFB_ETH_TYPE == m2->OFB_ETH_TYPE ))
		        		&& ((w & OFPFW_DL_SRC) || (!m1->OFB_ETH_SRC.compareTo(m2->OFB_ETH_SRC)))
		       			&& ((w & OFPFW_DL_DST) || (!m1->OFB_ETH_DST.compareTo(m2->OFB_ETH_DST)))
		       			&& ((w & OFPFW_NW_PROTO) || (m1->OFB_IP_PROTO == m2->OFB_IP_PROTO)) 
		       			&& ((w & OFPFW_NW_SRC_ALL) || (m1->OFB_IPV4_SRC == m2->OFB_IPV4_SRC))
		       			&& ((w & OFPFW_NW_DST_ALL) || (m1->OFB_IPV4_DST == m2->OFB_IPV4_DST)) ); 
		    }
		    //TCP or UDP Packet
		    else if (m1->OFB_IP_PROTO == 6 || m1->OFB_IP_PROTO == 17) {	
		    	return ( ((w & OFPFW_DL_TYPE) || (m1->OFB_ETH_TYPE == m2->OFB_ETH_TYPE ))
		        		&& ((w & OFPFW_DL_SRC) || (!m1->OFB_ETH_SRC.compareTo(m2->OFB_ETH_SRC)))
		       			&& ((w & OFPFW_DL_DST) || (!m1->OFB_ETH_DST.compareTo(m2->OFB_ETH_DST)))
		       			&& ((w & OFPFW_NW_PROTO) || (m1->OFB_IP_PROTO == m2->OFB_IP_PROTO)) 
		       			&& ((w & OFPFW_NW_SRC_ALL) || (m1->OFB_IPV4_SRC == m2->OFB_IPV4_SRC))
		       			&& ((w & OFPFW_NW_DST_ALL) || (m1->OFB_IPV4_DST == m2->OFB_IPV4_DST))
		       			&& ((w & OFPFW_TP_DST) || (m1->NW_DST == m2->NW_DST))
		       			&& ((w & OFPFW_TP_SRC) || (m1->NW_SRC == m2->NW_SRC)) ); 
		    }
		    else
		    	return 0;
		}
		else return 0;
	*/}
	else {
		return 0; 
	}

}


void Flow_Table::addEntry(oxm_basic_match *match, entry_data* data) {
    pair<multimap<oxm_basic_match, entry_data>::iterator, bool> ret;
    
    multimap<oxm_basic_match, entry_data>::iterator position;

    for (position = entry_map.begin(); position!=entry_map.end(); position++) {
    	if ((position->second.prior->priority) < (data->prior->priority)) {
    		entry_map.insert(position, pair<oxm_basic_match, entry_data>(*match, *data));
    		break;
    	}
    }
    if(position == entry_map.end())
    	entry_map.insert(pair<oxm_basic_match, entry_data>(*match, *data));
    
    EV << "Number of entries in "<< this->getFullName()<< ":"  <<entry_map.size() << endl;
    size = entry_map.size();

    //printFlowTable();
}

/* Print Flow Entry */
void Flow_Table::printFlowEntry (oxm_basic_match *match) {
	multimap<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
	EV << "Flow Entry:" << endl;
    while (iter != entry_map.end()) {
        oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards)) {
		    EV << "IN_PORT:" << iter->first.OFB_IN_PORT << endl;
		    EV << "ETH_SRC:" << iter->first.OFB_ETH_SRC << endl;
		    EV << "ETH_DST:" << iter->first.OFB_ETH_DST << endl;
		    EV << "ETH_TYPE:" << iter->first.OFB_ETH_TYPE <<endl;
		    EV << "SRC_IP:" << iter->first.OFB_IPV4_SRC <<endl;
		    EV << "DST_IP:" << iter->first.OFB_IPV4_DST <<endl;
		    EV << "IP_PROTO:" << iter->first.OFB_IP_PROTO <<endl;
		    EV << "DST PORT: " << iter->first.NW_DST <<endl;
		    EV << "SRC PORT: " << iter->first.NW_SRC <<endl;
		    EV << "COUNTERS:" << iter->second.counters->packet_count << endl;
		    EV << "PRIORITY:" << iter->second.prior->priority << endl;
        }
        iter++;
    }
}

/* Print Flow Table */
void Flow_Table::printFlowTable() {
	std::multimap<oxm_basic_match, entry_data>::iterator j = entry_map.begin();
	std::cout<<"===================================================================" << endl;
	std::cout << " [" << getParentModule()->getFullPath() <<" ] Flow Table:" << endl;
    while (j!=entry_map.end()) {
    	std::cout << "------------------------" << endl;
        std::cout << "IN_PORT:" << j->first.OFB_IN_PORT << endl;
        std::cout << "ETH_SRC:" << j->first.OFB_ETH_SRC << endl;
        std::cout << "ETH_DST:" << j->first.OFB_ETH_DST << endl;
        std::cout << "ETH_TYPE:" << j->first.OFB_ETH_TYPE <<endl;
        std::cout << "SRC_IP:" << j->first.OFB_IPV4_SRC <<endl;
        std::cout << "DST_IP:" << j->first.OFB_IPV4_DST <<endl;
        std::cout << "IP_PROTO:" << j->first.OFB_IP_PROTO <<endl;
        std::cout << "DST PORT: " << j->first.NW_DST <<endl;
        std::cout << "SRC PORT: " << j->first.NW_SRC <<endl;
        std::cout << "COUNTERS:" << j->second.counters->packet_count << endl;
        std::cout << "PRIORITY:" << j->second.prior->priority << endl;
        std::cout << "OUTPORT:" << j->second.instruc->actions[0].port << endl;
        j++;
    }
}

bool Flow_Table::deleteEntry(oxm_basic_match *match)
{
	EV << "Delete Entry:" << endl;
    EV << "Number of entries in "<< this->getFullName()<< ":"  << entry_map.size() << endl;

    multimap<oxm_basic_match, entry_data>::iterator iter = entry_map.end();
    while (iter != entry_map.begin()) {
        oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards))
        {
            entry_map.erase(iter);
            delete match;
            size = entry_map.size();
            EV << "Number of entries in "<< this->getFullName()<< ":"  <<entry_map.size() << endl;
            return true;
        }
        iter--;
    }
    EV << "Number of entries in "<< this->getFullName()<< ":"  <<entry_map.size() << endl;
    return false;
}



bool Flow_Table::lookup(oxm_basic_match *match)
{
    EV << "lookup \n";
    map<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
    while (iter != entry_map.end()) {    
        oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards)) {
        	updateCounters (iter->second.counters);
            return true;
        }
        iter++;
    }
    return false;
}

ofp_action_output *Flow_Table::returnAction(oxm_basic_match *match)
{
    map<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
    while (iter != entry_map.end()) {
        oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards))
            return &iter->second.instruc->actions[0];
        iter++;
    }
    return NULL;
}

//return the whole flow table
ofp_flow_stats *Flow_Table::returnStats() {
	int size =0;
	if (!entry_map.empty()) {
		size = entry_map.size();
	}
	ofp_flow_stats *flow_stats = new ofp_flow_stats[size+1];
	//dummy field in order to keep only the size of the flow entry table.
	flow_stats[0].size = size;
	
	map<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
	int i=1;
	while (iter != entry_map.end()) {
		oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
		flow_stats[i].match = *m;         
		flow_stats[i].actions = &iter->second.instruc->actions[0];
		flow_stats[i].packet_count = iter->second.counters->packet_count;
		//reset the counters per flow
		iter->second.counters->packet_count = 0;
		flow_stats[i].size = size;
		iter++;
		i++;
	}

	return flow_stats;
}

//function which returns statistics for a specific flow
ofp_flow_stats *Flow_Table::returnStats(oxm_basic_match *match) {
	ofp_flow_stats *flow_stats = new ofp_flow_stats[1];
	map<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
	while (iter != entry_map.end()) {
		oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards)) {
        	flow_stats[0].match = *m; 
			flow_stats[0].actions = &iter->second.instruc->actions[0];
			flow_stats[0].packet_count = iter->second.counters->packet_count;
        }
		iter++;
	}
	return flow_stats;
}

void Flow_Table::updateCounters(flow_table_counters *cnt)
{
	cnt->packet_count++;
	EV << "[FlowTable] (Update Counters)" << endl;
}

int Flow_Table::getPacketCount(oxm_basic_match *match)
{
	map<oxm_basic_match, entry_data>::iterator iter = entry_map.begin();
    while (iter != entry_map.end()) {    
        oxm_basic_match *m = const_cast<oxm_basic_match*> (&iter->first);
        if (flow_fields_match(match, m, iter->first.wildcards)) 
        	return iter->second.counters->packet_count;
        iter++;
    }
    return -1;
}


void Flow_Table::deleteFlowTable() {
    multimap<oxm_basic_match, entry_data>::iterator iterBegin = entry_map.begin();
    multimap<oxm_basic_match, entry_data>::iterator iterEnd = entry_map.end();
    entry_map.erase(iterBegin, iterEnd);
}


void Flow_Table::handleMessage(cMessage *msg)
{
    opp_error("This module doesn't process messages");
}
