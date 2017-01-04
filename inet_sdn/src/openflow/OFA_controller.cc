#include <omnetpp.h>
#include <OFA_controller.h>

#include "list"
#include "IPv4Datagram.h"
#include "GenericAppMsg_m.h"

#include "Hub.h"
#include "Switch.h"
#include "Forwarding.h"
#include "OF_Wrapper.h"
#include "OFP_Packet_Out_m.h"
#include "TCPConnection.h"
#include "AttackDetection.h"
#include "Mitigation.h"

#include "openflow.h"
#include "Open_Flow_Message_m.h"
#include "OFP_Flow_Mod_m.h"
#include "OFP_Features_Request_m.h"
#include "OFP_Features_Reply_m.h"
#include "OFP_Flow_Removed_m.h"
#include "OFP_Flow_Stats_Request_m.h"
#include "OFP_Flow_Stats_Reply_m.h"
#include "OFP_Echo_Request_m.h"
#include "OFP_Echo_Reply_m.h"
#include "OFP_Port_Status_m.h"

using namespace std;

Define_Module(OFA_controller);

OFA_controller::OFA_controller() {
	reqFlowStats = NULL;
}

OFA_controller::~OFA_controller() {
}

void OFA_controller::initialize()
{
    const char *address = par("address");
    int port = par("port");
    serviceTime = par("serviceTime");
	statsInterval = par("statsInterval");
	
    // TCP socket; listen on incoming connections
    socket1.setOutputGate(gate("tcpOut"));
    socket1.setDataTransferMode(TCP_TRANSFER_OBJECT);
    socket1.bind(address[0] ? IPvXAddress(address) : IPvXAddress(), port);
    socket1.listen();
    


    PacketInSignalId = registerSignal("PacketIn");
    connIDSignal = registerSignal("connID");
    
    // <A.S>
    errorSignal = registerSignal("ErrorDetected");
	
    busy = false;
    
    // schedule self messages
    reqFlowStats = new cMessage("RequestFlowStats");
    reqFlowStats->setKind(STATS_REQUEST);
    scheduleAt(simTime()+statsInterval, reqFlowStats);
	
	// <A.S>
	echoRequestMsg = new cMessage("EchoRequest");
    echoRequestMsg->setKind(ECHO_REQUEST);
	scheduleAt(simTime()+par("echoInterval"), echoRequestMsg);
	
	allActive = true;
	allCollectStats = true;
	
	cModule *ITModule = getParentModule()->getSubmodule("attackDetection");
    attackDetection = check_and_cast<AttackDetection *>(ITModule);
    
    getParentModule()->subscribe("Mitigation", this);
	
}

TCPSocket *OFA_controller::findSocketFor(cMessage *msg) {
    TCPCommand *ind = dynamic_cast<TCPCommand *>(msg->getControlInfo());
    if (!ind)
        opp_error("TCPSocketMap: findSocketFor(): no TCPCommand control info in message (not from TCP?)");
    int connId = ind->getConnId();
    SocketMap::iterator i = socketMap.find(connId);
    ASSERT(i==socketMap.end() || i->first==i->second->getConnectionId());
    return (i==socketMap.end()) ? NULL : i->second;
}

// <A.S>
void OFA_controller::receiveSignal(cComponent *src, simsignal_t id, cObject *obj) {
    Enter_Method_Silent();
	string signalName(getSignalName(id));	
	if (signalName == "Mitigation") { 
		if (dynamic_cast<OF_Wrapper *>(obj) != NULL) {
		    OF_Wrapper *wrapper = (OF_Wrapper *) obj;
		    oxm_basic_match *match = wrapper->match;
		    installProtectionRule(match);
		}
	}
}


// self messages used to implement service time
void OFA_controller::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        switch(msg->getKind()) {
            case STATS_REQUEST:
                flowStatsRequest();
			    break;
			case CHECK_STATS:
                checkStatsReplies();
                break;
            case ECHO_REQUEST:
                sendEchoRequest();
                break;
            case CHECK:
                checkEchoReplies();
                break;
            default: 
                cMessage *data_msg = (cMessage *) msg->getContextPointer();
		        processQueuedMsg(data_msg);	    		    
		        if (msg_list.empty()) {
		            busy = false;
		        }
		        else {
		            cMessage *msgfromlist = msg_list.front();
		            msg_list.pop_front();
		            char buf[80];
		            sprintf(buf, " %d pakets in queue", msg_list.size());
		            getParentModule()->getDisplayString().setTagArg("t", 0, buf);
		            std::list<cMessage *>::iterator i = msg_list.begin();
		            while (i!=msg_list.end()) {
		            	EV << (*i)->getFullPath() << endl;
		                i++;
		            }
		            cMessage *event = new cMessage("event");
		            event->setContextPointer(msgfromlist);
		            scheduleAt(simTime()+serviceTime, event);
		        }
        }
        delete msg;
    }
    else
    {
        if (busy) {
            msg_list.push_back(msg);
        }
        else {
            busy = true;
            cMessage *event = new cMessage("event");
            event->setContextPointer(msg);
            scheduleAt(simTime()+serviceTime, event);
        }
    }
}



void OFA_controller::processQueuedMsg(cMessage *data_msg) {
    if (data_msg->getKind() == 3) { //connection has been established

        TCPSocket *socket = findSocketFor(data_msg);
        if(!socket)
        {
            socket = new TCPSocket(data_msg);
            socket->setOutputGate(gate("tcpOut"));
       //     socket->setCallbackObject(this, NULL);
            ASSERT(socketMap.find(socket->getConnectionId())==socketMap.end());
            socketMap[socket->getConnectionId()] = socket;
            TCPConnectInfo *info = check_and_cast<TCPConnectInfo *>(data_msg->getControlInfo());

            IPv4Address ip_src =  (IPv4Address) (info->getRemoteAddr().get4());
            OF_Wrapper *wrapper = new OF_Wrapper();
            wrapper->connID = socket->getConnectionId();
            wrapper->ip_src = &ip_src;
            emit(connIDSignal, wrapper);           
        }

        handshake(data_msg);
    }
   
    if (dynamic_cast<Open_Flow_Message *>(data_msg) != NULL) {
        Open_Flow_Message *of_msg = (Open_Flow_Message *)data_msg;
        ofp_type type = (ofp_type)of_msg->getHeader().type;
        OF_Wrapper *wrapper = new OF_Wrapper();
        switch (type) {
		    case OFPT_PACKET_IN:
		        EV << "packet-in message from switch\n";
		        wrapper->msg = of_msg;
		        EV << getSignalName(PacketInSignalId) << ": " << hasListeners(PacketInSignalId) << " listeners\n";
		        emit(PacketInSignalId, wrapper);
		        break;
		    case OFPT_FEATURES_REPLY:
		        handleFeaturesReply(of_msg);
		        break;
		    case OFPT_ECHO_REPLY:
		        handleEchoReply(of_msg);
		        break;
		    case OFPT_FLOW_REMOVED:
		    	handleFlowRemovedMessage(of_msg);
		        break;
		    case OFPT_STATS_REPLY:
		    	handleFlowStatsReply(of_msg);
		    	break;
		    case OFPT_PORT_STATUS:
                handlePortStatus(of_msg);
		        break;
		    default:
		        break;
        }
    }
    delete data_msg;
}

void OFA_controller::handshake(cMessage *msg) {
    OFP_Features_Request *featuresRequest = new OFP_Features_Request("FeaturesRequest");
    featuresRequest->getHeader().version = OFP_VERSION;
    featuresRequest->getHeader().type = OFPT_FEATURES_REQUEST;
    featuresRequest->setByteLength(1);
    featuresRequest->setKind(TCP_C_SEND);

    TCPSocket *socket = findSocketFor(msg);
    socket->send(featuresRequest);

}

void OFA_controller::handleFeaturesReply(Open_Flow_Message *of_msg) {
    EV << "OFA_controller::handleFeaturesReply" << endl;
    //OFP_Features_Reply *featuresReply = (OFP_Features_Reply *) of_msg;
}

void OFA_controller::handlePortStatus(Open_Flow_Message *of_msg) {
    //identify the switch
    //deletes the flow table of the switch
    //recalculate topo
    OFP_Port_Status *msg = (OFP_Port_Status *)of_msg;
    switch(msg->getReason()) {
        case OFPPR_DELETE: {
            cout<< "[" << simTime() << " ] port donwn " << endl;
            int datapathId = msg->getDatapath_Id();
            ofp_port_status *port_status = new ofp_port_status();
            *port_status = msg->getPort_status();
            uint16_t portNo = port_status->desc.port_no;
            
            //erase the flow table of the switch
            SocketMap::iterator i = socketMap.begin();
	        while (i != socketMap.end() ) {
	            OFP_Flow_Mod *flow_mod_msg = new OFP_Flow_Mod("flow_mod");
                flow_mod_msg->getHeader().version = OFP_VERSION;
                flow_mod_msg->getHeader().type = OFPT_FLOW_MOD;
                flow_mod_msg->setCommand(OFPFC_DELETE);
                flow_mod_msg->setByteLength(1);
                flow_mod_msg->setKind(TCP_C_SEND);
		        
		        TCPSocket *socket;
		        socket = i->second;
                socket -> send(flow_mod_msg);
		        socket = NULL;
		        i++;
	        }
            //send signal to the control. app to recalculate the topo
            if (hasListeners(errorSignal)) {
                OF_Wrapper *wrapper = new OF_Wrapper();
                emit(errorSignal,wrapper);
            }
        }
        default:
            break;
    }

}

       
void OFA_controller::sendEchoRequest() {
    allActive = false;
    EV << "OFA_controller::handleEchoRequest" << endl;
    SocketMap::iterator i = socketMap.begin();
	while (i != socketMap.end() ) {
		OFP_Echo_Request *echoRequest= new OFP_Echo_Request("EchoRequest");
		echoRequest->getHeader().type = OFPT_ECHO_REQUEST;
		echoRequest->setByteLength(1);
		echoRequest->setKind(TCP_C_SEND);
		TCPSocket *socket;
		socket = i->second;
        socket->send(echoRequest);
		socket = NULL;
		i++;
	} 

    //check after 1 sec which switches have replier. if not all, then re-calculate topo
    cMessage *checkMsg = new cMessage("CheckEchoReplies");
    checkMsg->setKind(CHECK);  
    scheduleAt(simTime()+1, checkMsg);
}

void OFA_controller::handleEchoReply(Open_Flow_Message *of_msg) {
	EV << "OFA_controller::handleEchoReply" << endl;
    OFP_Echo_Reply *echoReply = (OFP_Echo_Reply *) of_msg;
	int datapathId = echoReply->getDatapath_id();
	
	//activeSwitches.push_back(datapathId);
	activeSwitches.push_back(findSocketFor(of_msg)->getConnectionId());
	if(activeSwitches.size() == socketMap.size()) {
	    allActive = true;
	    activeSwitches.clear();
	}
}

void OFA_controller::checkEchoReplies() {
    
    if (activeSwitches.size() != socketMap.size() && !allActive) { //be sure that no new echo request has been sent
        
        findDisconnectedSwitch(activeSwitches); //close the socket of the disconnected switch
        
        activeSwitches.clear();
        
        //reset all flows to all the switches
        cout << "[" << simTime() << " ] OFA_controller::sendFlowModMessage:DELETE" << endl;
        
        SocketMap::iterator i = socketMap.begin();
	    while (i != socketMap.end() ) {
	        OFP_Flow_Mod *flow_mod_msg = new OFP_Flow_Mod("flow_mod");
            flow_mod_msg->getHeader().version = OFP_VERSION;
            flow_mod_msg->getHeader().type = OFPT_FLOW_MOD;
            flow_mod_msg->setCommand(OFPFC_DELETE);
            flow_mod_msg->setByteLength(1);
            flow_mod_msg->setKind(TCP_C_SEND);
		    
		    TCPSocket *socket;
		    socket = i->second;
            socket -> send(flow_mod_msg);
		    socket = NULL;
		    i++;
	    }
	    
        //send signal to the control. app to recalculate the topo
        if (hasListeners(errorSignal)) {
            OF_Wrapper *wrapper = new OF_Wrapper();
            emit(errorSignal,wrapper);
        }
        
        //if an error has been detected, re-calculate the topo and re schedule the messages in the right order to keep the period alligned
        cMessage *echoReqMsg = new cMessage("EchoRequest");
        echoReqMsg->setKind(ECHO_REQUEST);
        scheduleAt(simTime()+par("echoInterval")+1, echoReqMsg);   
    
    }
    else { //re-schedule the self message
        cMessage *echoReqMsg = new cMessage("EchoRequest");
        echoReqMsg->setKind(ECHO_REQUEST);
        scheduleAt(simTime()+par("echoInterval")-1, echoReqMsg);   
    }

}

void OFA_controller::findDisconnectedSwitch(std::vector<int> activeSwitches) {    
    SocketMap::iterator i = socketMap.begin();
    while (i != socketMap.end()) {
        std::vector<int>::iterator it = find (activeSwitches.begin(), activeSwitches.end(), i->first);
        if (it == activeSwitches.end()) {
            i->second->close();           
            socketMap.erase(i);
        }
        i++;
    }
}

void OFA_controller::handleFlowRemovedMessage(Open_Flow_Message *of_msg) {
	EV << "OFA_controller::handleFlowRemoved" << endl;
	//OFP_Flow_Removed *flowRemoved = (OFP_Flow_Removed *) of_msg;
	//int datapath = flowRemoved->getDatapath_id();
	//int reason = static_cast<std::underlying_type<ofp_flow_removed_reason>::type>(flowRemoved->getReason());
	//oxm_basic_match match = flowRemoved->getMatch();
	
}

void OFA_controller::flowStatsRequest() {
    allCollectStats = false;
	EV << "OFA_controller::flowStatsRequest" << endl;
	SocketMap::iterator i = socketMap.begin();
	while (i != socketMap.end() ) {
		OFP_Flow_Stats_Request *flowStatsRequest= new OFP_Flow_Stats_Request("FlowStatsRequest");
		flowStatsRequest->getHeader().type = OFPT_STATS_REQUEST;
		flowStatsRequest->setByteLength(1);
		flowStatsRequest->setKind(TCP_C_SEND);
		TCPSocket *socket = NULL;
		socket = i->second;
		socket -> send(flowStatsRequest);
		socket = NULL;
		i++;
	}
	
    //check after 1 sec which switches have replier. if not all, then re-calculate topo
    cMessage *checkMsg = new cMessage("CheckStatsReplies");
    checkMsg->setKind(CHECK_STATS);  
    scheduleAt(simTime()+1, checkMsg);
    
}

void OFA_controller::handleFlowStatsReply (Open_Flow_Message *of_msg) {
	EV << "OFA_controller::handleFlowStatsReply" << endl;
		
	OFP_Flow_Stats_Reply *flowStatsReply = (OFP_Flow_Stats_Reply *) of_msg;
	int datapathId = flowStatsReply->getDatapath_Id();
	int size = flowStatsReply->getFlowStatsArrayArraySize();

	ofp_flow_stats *stats = new ofp_flow_stats [size];
	if (size == 0)
		stats->size = 0;

	for (int i=0; i < size; i++) //extract info from the OF msg
		stats = flowStatsReply->getFlowStatsArray(i);
	
	//map with struct. size val is in the struct
	StatsMap::iterator it;
	it = collectedStats.find(datapathId);
	if (it == collectedStats.end() ) { 
		collectedStats.insert(make_pair(datapathId, stats));
	}
	
	//call detection module when all switches have replied
	if (collectedStats.size() == socketMap.size()) { 
	    allCollectStats = true;
		attackDetection->dataAnalysis(collectedStats);
		collectedStats.clear();
	}
}

void OFA_controller::checkStatsReplies() {
    if (collectedStats.size() != socketMap.size() && !allCollectStats) {
        collectedStats.clear();
    }
    else { //re-schedule the self message
        cMessage *echoReqMsg = new cMessage("EchoRequest");
        echoReqMsg->setKind(ECHO_REQUEST);
        scheduleAt(simTime()+par("echoInterval")-1, echoReqMsg);   
    }
}

void OFA_controller::floodPacket(uint32_t buffer_id, OFP_Packet_In *packet_in_msg, int connID)
{
    EV << "OFA_controller::floodPacket" << endl;
    OFP_Packet_Out *packetOut = new OFP_Packet_Out("packetOut");
    packetOut->getHeader().version = OFP_VERSION;
    packetOut->getHeader().type = OFPT_PACKET_OUT;
    packetOut->setBuffer_id(buffer_id);
    packetOut->setByteLength(1);
    if (buffer_id == OFP_NO_BUFFER) {
        EthernetIIFrame *frame =  dynamic_cast<EthernetIIFrame *>(packet_in_msg->decapsulate());
        packetOut->encapsulate(frame);
    }
    ofp_action_output *action_output = new ofp_action_output();
    action_output->port = OFPP_FLOOD;
    EV << "outport: " << action_output->port << endl;
    packetOut->setActionsArraySize(1);
    packetOut->setActions(0, *action_output);

    ofp_action_output *output= &packetOut->getActions(0);
    EV << "outport: " << output->port << endl;
    packetOut->setKind(TCP_C_SEND);
    TCPSocket *socket = findSocketFor(packet_in_msg);
    socket->send(packetOut);

}


void OFA_controller::sendPacket(uint32_t buffer_id, OFP_Packet_In *packet_in_msg, uint32_t outport, int connID)
{
    EV << "OFA_controller::sendPacket" << endl;
    OFP_Packet_Out *packetOut = new OFP_Packet_Out("packetOut");
    packetOut->getHeader().version = OFP_VERSION;
    packetOut->getHeader().type = OFPT_PACKET_OUT;
    packetOut->setBuffer_id(buffer_id);
    packetOut->setByteLength(1);
    if (buffer_id == OFP_NO_BUFFER) {
        EthernetIIFrame *frame =  dynamic_cast<EthernetIIFrame *>(packet_in_msg->decapsulate());
        packetOut->encapsulate(frame);
    }

    ofp_action_output *action_output = new ofp_action_output();
    action_output->port = outport;
    packetOut->setActionsArraySize(1);
    packetOut->setActions(0, *action_output);

    packetOut->setKind(TCP_C_SEND);
    
    TCPSocket *socket = findSocketFor(packet_in_msg);
    
    
    socket->send(packetOut);
}

void OFA_controller::sendFlowModMessage(ofp_flow_mod_command mod_com, oxm_basic_match *match, uint32_t outport, int connID) {
    EV << "OFA_controller::sendFlowModMessage" << endl;
    OFP_Flow_Mod *flow_mod_msg = new OFP_Flow_Mod("flow_mod");
    flow_mod_msg->getHeader().version = OFP_VERSION;
    flow_mod_msg->getHeader().type = OFPT_FLOW_MOD;
    flow_mod_msg->setCommand(OFPFC_ADD);
    flow_mod_msg->setMatch(*match);
    flow_mod_msg->setByteLength(1);
    flow_mod_msg->setPriority(1);
    ofp_action_output *action_output = new ofp_action_output();
    action_output->port = outport;
    flow_mod_msg->setActionsArraySize(1);
    flow_mod_msg->setActions(0, *action_output);

    flow_mod_msg->setKind(TCP_C_SEND);

    TCPSocket *socket = socketMap[connID];
    socket->send(flow_mod_msg);
}

double OFA_controller::returnStatsInterval() {
	return statsInterval;
}

//install mitigation rule to all the switches
void OFA_controller::installProtectionRule(oxm_basic_match *match) {
	EV << "OFA_controller::installProtectionRule" << endl;
	SocketMap::iterator i = socketMap.begin();
	while (i != socketMap.end() ) {
		OFP_Flow_Mod *flow_mod_msg = new OFP_Flow_Mod("flow_mod");
		flow_mod_msg->getHeader().version = OFP_VERSION;
		flow_mod_msg->getHeader().type = OFPT_FLOW_MOD;
		flow_mod_msg->setCommand(OFPFC_ADD);
		flow_mod_msg->setMatch(*match);
		
		flow_mod_msg->setByteLength(1);
		flow_mod_msg->setPriority(2);
		
		//Empty action List::indication of DROP action
		flow_mod_msg->setActionsArraySize(0);
		
		flow_mod_msg->setKind(TCP_C_SEND);
		//send it to all the switches
		TCPSocket *socket = NULL;
		socket = i->second;
		socket -> send(flow_mod_msg);
		socket = NULL;
		i++;
	}
}

