cplusplus {{
    #include "openflow.h"
    #include "Open_Flow_Message_m.h"
    typedef ofp_flow_stats *FlowStatsPtr;
}};

packet Open_Flow_Message;
struct ofp_header;
struct oxm_basic_match;
struct ofp_action_output;
struct ofp_flow_stats;
class noncobject FlowStatsPtr;

packet OFP_Flow_Stats_Reply extends Open_Flow_Message
{
	int datapath_Id;
	FlowStatsPtr flowStatsArray[];
}
