cplusplus {{
    #include "openflow.h"
    #include "Open_Flow_Message_m.h"
}};


packet Open_Flow_Message;
struct oxm_basic_match;

//Individual flow statistics request message
//The controller uses this message to query individual flow statistics.
packet OFP_Flow_Stats_Request extends Open_Flow_Message
{
		oxm_basic_match match;
		int datapath_id;
		uint8_t table_id;
		uint32_t out_port;
}
