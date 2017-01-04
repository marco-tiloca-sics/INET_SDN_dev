cplusplus {{
    #include "openflow.h"
    #include "Open_Flow_Message_m.h"
}};

packet Open_Flow_Message;// Modifications to a flow table by the controller.
struct ofp_port_status;

packet OFP_Port_Status extends Open_Flow_Message
{
    ofp_port_status port_status;
    uint8_t reason;
    int datapath_Id;
}
