/*
 * Flow_Table.h
 *
 *  Created on: 20.04.2012
 *      Author: rachor
 */

#ifndef FLOW_TABLE_H_
#define FLOW_TABLE_H_

#include <list>
#include <map>
#include <openflow.h>

using namespace __gnu_cxx;


struct flow_table_counters {
	int packet_count;
};

struct flow_table_instructions {
    ofp_action_output actions[1];
};

struct flow_table_priority {
	int priority;
};

struct entry_data {
    flow_table_counters *counters;
    flow_table_instructions *instruc;
    flow_table_priority *prior;
};

struct ltmatch
{
    bool operator() (const oxm_basic_match m1, const oxm_basic_match m2) const
    {
        if(m1.OFB_IN_PORT == m2.OFB_IN_PORT){
            return false;
        }else if(m1.OFB_ETH_TYPE == m2.OFB_ETH_TYPE){
            return false;
        }else if(m1.OFB_ETH_SRC == m2.OFB_ETH_SRC){
            return false;
        }else if(m1.OFB_ETH_DST == m2.OFB_ETH_DST){
            return false;
        }else if (m1.wildcards != m2.wildcards){
            return false;
        }
        else
        return true;
    }
};
class Flow_Table : public cSimpleModule
{
public:
    Flow_Table();
    void addEntry(oxm_basic_match *match, entry_data* entry);
    bool deleteEntry(oxm_basic_match *match);
    bool lookup(oxm_basic_match *match);
    ofp_action_output *returnAction(oxm_basic_match *match);
	int getPacketCount(oxm_basic_match *match);
	void printFlowEntry (oxm_basic_match *match);
	void printFlowTable();
	ofp_flow_stats *returnStats(oxm_basic_match *match);
	ofp_flow_stats *returnStats();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void updateCounters (flow_table_counters *cnt);

private:
    std::multimap<oxm_basic_match, entry_data, ltmatch> entry_map;
    int size;
};





#endif /* FLOW_TABLE_H_ */
