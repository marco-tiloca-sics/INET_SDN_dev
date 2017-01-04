cplusplus {{
    #include "openflow.h"
    #include "Open_Flow_Message_m.h"
}};


packet Open_Flow_Message;

packet OFP_Echo_Reply extends Open_Flow_Message
{
    int datapath_id;
}
