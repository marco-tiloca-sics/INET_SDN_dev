/**
 * @file	Change.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @version	0.0.7
 * @date	2015 may 04
 */

#include "Change.h"
#include "seapputils.h"
#include "omnetpp.h"

#include "UDPControlInfo.h"
#include "TCPCommand_m.h"
#include "IPv4ControlInfo.h"
#include "Ieee802Ctrl_m.h"
#include "IPv4Datagram.h"
#include "PPPFrame_m.h"
#include "EtherFrame_m.h"
#include "IdealAirFrame_m.h"
#include "AirFrame_m.h"

#include "IInterfaceTable.h"
#include "InterfaceTableAccess.h"
#include "IPv4InterfaceData.h"


// TODO change value in valueName

Change::Change(const string fieldName, const string value) : ActionBase(action_t::CHANGE)
{
	vector<string> tokens;
	tokenize(tokens, fieldName, '.');

	if ((tokens[0] == "controlInfo") || (tokens[0]=="sending") || tokens[0]=="attackInfo") {
		this->externalInfo.assign(tokens[0]);
		involvedLayer = NONE_LAYER;
	}
	else {
		this->externalInfo.assign("none");
		involvedLayer = layertoi(tokens[0]);
	}

	this->fieldName.assign(tokens[1]);
	this->value.assign(value);	
}


Change::~Change()
{  
}


string Change::getFieldName() const
{
	return fieldName;
}


string Change::getValue() const
{
	return value;
}


void Change::execute(cMessage** packetToChange, string value)
{
	// change the value of the packet's field
	if (externalInfo == "none") {
		executeOnField(packetToChange, value);
	}
	else {
		executeOnExternalInfo(packetToChange, value);		
	}	
}

// <A.S>: Set payload
void Change::execute(cMessage **packetToChange, cMessage **payload) {
    int payloadLayer = getPacketLayer((cPacket*)(*payload)); //correct!!
    int packetLayer = getPacketLayer((cPacket*)(*packetToChange));
    
    //error_check
    if (payloadLayer < packetLayer) {
        string errorMsg = "[void Change::execute(cMessage**, string, cMessage*)] Error, cannot set packet of lower layer as payload";
		opp_error(errorMsg.c_str());
    }
    else if (payloadLayer > packetLayer+1) {
        string errorMsg = "[void Change::execute(cMessage**, string, cMessage*)] Error, cannot set packet of layer ";
        errorMsg.append(to_string(payloadLayer));
        errorMsg.append(" as payload of packet layer ");
        errorMsg.append(to_string(packetLayer));
		opp_error(errorMsg.c_str());
    }
    else {
        cMessage* substitutePacket = nullptr;
	    cMessage* encapsulatedPacket = nullptr;

	    // work on the clone of the original packet
	    substitutePacket = (cMessage*) hardCopy((cPacket*)(*packetToChange));
	    encapsulatedPacket = (cMessage*) hardCopy((cPacket*)(*payload));
	    delete *payload;
	    *payload = nullptr;
	    
	    ((cPacket*)substitutePacket) -> encapsulate((cPacket*)encapsulatedPacket);
        
        // replace the original packet with its modified clone	
	    delete *packetToChange;
	    *packetToChange = substitutePacket;
    }
    
}
	
	
void Change::executeOnField(cMessage** packetToChange, string value)
{
	cClassDescriptor* descriptor; 
	cMessage* substitutePacket = nullptr;
	cMessage* encapsulatedPacket = nullptr;
	string packetClassName = "";
	int packetLayer = getPacketLayer((cPacket*)(*packetToChange));
	int fieldIndex;
	bool isRandom = false;

	// work on the clone of the original packet
	substitutePacket = (cMessage*) hardCopy((cPacket*)(*packetToChange));
	encapsulatedPacket = substitutePacket;	
	while (packetLayer < involvedLayer) {
		encapsulatedPacket = ((cPacket*) encapsulatedPacket)->getEncapsulatedPacket();
		if (encapsulatedPacket == nullptr) {
			string errorMsg = "[void Change::execute(cMessage**, string)] Error, can't find the right encapsulated packet, check your packet filter specifications";
			opp_error(errorMsg.c_str());
		}
		packetLayer = getPacketLayer((cPacket*)encapsulatedPacket);
	}

	descriptor = cClassDescriptor::getDescriptorFor(encapsulatedPacket);
	fieldIndex = descriptor->findField(encapsulatedPacket, fieldName.c_str());	
	// can't find the specified field
	if (fieldIndex == -1) {
		string errorMsg = "[void Change::execute(cMessage**, string)] Error, can't find the field '";
        errorMsg.append(fieldName);
        errorMsg.append("'");
		opp_error(errorMsg.c_str());
	}

    // <A.S>
    if (isRandomValue(value)) {
        value = generateRandomValue(descriptor->getFieldTypeString(encapsulatedPacket, fieldIndex));
        isRandom = true;
    }
  
	//explicit handling of Ipv4Datagram src & dst fields as well as EtherFrame src & dst fields
	packetClassName = encapsulatedPacket->getClassName();

    if (fieldName == "srcAddress" && packetClassName == "IPv4Datagram") {
        if (isRandom)
	        (check_and_cast<IPv4Datagram*> (encapsulatedPacket))->setSrcAddress(IPv4Address(stoull(value)));
		else
		    (check_and_cast<IPv4Datagram*> (encapsulatedPacket))->setSrcAddress(IPv4Address(value.c_str()));
    }
	else if (fieldName == "destAddress" && packetClassName == "IPv4Datagram")
		(check_and_cast<IPv4Datagram*> (encapsulatedPacket))->setDestAddress(IPv4Address(value.c_str()));
	else if (fieldName == "src" && packetClassName == "EthernetIIFrame") {
	    if (isRandom)
	        (check_and_cast<EthernetIIFrame*> (encapsulatedPacket))->setSrc(MACAddress(stoull(value)));
		else
		    (check_and_cast<EthernetIIFrame*> (encapsulatedPacket))->setSrc(MACAddress(value.c_str()));
	}
	else if (fieldName == "dest" && packetClassName == "EthernetIIFrame")
	    if (isRandom)
	        (check_and_cast<EthernetIIFrame*> (encapsulatedPacket))->setDest(MACAddress(stoull(value)));
		else
		    (check_and_cast<EthernetIIFrame*> (encapsulatedPacket))->setDest(MACAddress(value.c_str()));
	else
		// edit the value of the specified field
		descriptor->setFieldAsString(encapsulatedPacket, fieldIndex, 0, value.c_str()); //fields of addresses are not handled!    

	// replace the original packet with his modified clone	
	delete *packetToChange;
	*packetToChange = substitutePacket;
}

	
void Change::executeOnExternalInfo(cMessage** packetToChange, string value)
{	
	// change control info fields
	if (externalInfo == "controlInfo") {
		string className;
		cObject* controlInfo = (*packetToChange)->getControlInfo();
		
		if (controlInfo == nullptr) {
			opp_error("[void Change::executeOnExternalInfo(cMessage**, string)] Error, can't find the controlInfo object");
		}

		// get class name of the controlinfo object
		className.assign(controlInfo->getClassName());
		
		// <A.S>
		bool isRandom = false;
        if (isRandomValue(value)) {
            value = generateRandomValue(getValueType(value).c_str()); // ip, mac, int, long.. 
            isRandom = true;
        }

        //change the packet size!
        if (fieldName ==  "packetSize") {
            if ((*packetToChange)->isPacket())
                (check_and_cast<cPacket*> (*packetToChange))-> setByteLength(atoi(value.c_str()));
            return;
        }
        
		// handle UDP control info
		if (className.find("UDP") != std::string::npos) {
			
			// UDPControlInfo (base class)
			if (fieldName == "sockId") {
				(check_and_cast<UDPControlInfo*> (controlInfo))->setSockId(atoi(value.c_str()));
				return;
			}
			
			// UDPSendCommand:UDPControlInfo
			if (className == "UDPSendCommand") {
			
				if (fieldName == "destAddr") {
					IPvXAddress destAddr(value.c_str());
					(check_and_cast<UDPSendCommand*> (controlInfo))->setDestAddr(destAddr);
					return;
				}
			
				if (fieldName == "srcAddr") {
					IPvXAddress srcAddr(value.c_str());
					(check_and_cast<UDPSendCommand*> (controlInfo))->setSrcAddr(srcAddr);
					return;
				}
				
				if (fieldName == "destPort") {
					(check_and_cast<UDPSendCommand*> (controlInfo))->setDestPort(atoi(value.c_str()));
					return;
				}
				
				if (fieldName == "interfaceId") {
					(check_and_cast<UDPSendCommand*> (controlInfo))->setInterfaceId(atoi(value.c_str()));
					return;
				}

				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// UDPBindCommand:UDPControlInfo
			if (className == "UDPBindCommand") {
			
				if (fieldName == "localAddr") {
					IPvXAddress localAddr(value.c_str());
					(check_and_cast<UDPBindCommand*> (controlInfo))->setLocalAddr(localAddr);
					return;
				}
			
				if (fieldName == "localPort") {
					(check_and_cast<UDPBindCommand*> (controlInfo))->setLocalPort(atoi(value.c_str()));
					return;
				}
			
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// UDPConnectCommand:UDPControlInfo
			if (className == "UDPConnectCommand") {
			
				if (fieldName == "remoteAddr") {
					IPvXAddress remoteAddr(value.c_str());
					(check_and_cast<UDPConnectCommand*> (controlInfo))->setRemoteAddr(remoteAddr);
					return;
				}
			
				if (fieldName == "remotePort") {
					(check_and_cast<UDPConnectCommand*> (controlInfo))->setRemotePort(atoi(value.c_str()));
					return;
				}
			
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// UDPCloseCommand:UDPControlInfo
			if (className == "UDPCloseCommand") {
				// UDPCloseCommand hasn't fields
			}
			
			// UDPDataIndication:UDPControlInfo
			if (className == "UDPDataIndication") {
				
				if (fieldName == "srcAddr") {
					IPvXAddress srcAddr(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setSrcAddr(srcAddr);
					return;
				}
			
				if (fieldName == "destAddr") {
					if (value == "random")
						value = generateRandomValue("IPvXAddress");
					IPvXAddress destAddr(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setDestAddr(destAddr);
					return;
				}
				
				if (fieldName == "srcPort") {
					int srcPort = atoi(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setSrcPort(srcPort);
					return;
				}
				
				if (fieldName == "destPort") {
					int destPort = atoi(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setDestPort(destPort);
					return;
				}
				
				if (fieldName == "ttl") {
					int ttl = atoi(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setTtl(ttl);
					return;
				}
			
				if (fieldName == "interfaceId") {
					int interfaceId = atoi(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setInterfaceId(interfaceId);
					return;
				}
				
				if (fieldName == "typeOfService") {
					unsigned char typeOfService = *(value.c_str());
					(check_and_cast<UDPDataIndication*> (controlInfo))->setTypeOfService(typeOfService);
					return;
				}

				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// UDPErrorIndication:UDPControlInfo
			if (className == "UDPErrorIndication") {
				
				if (fieldName == "srcAddr") {
					IPvXAddress srcAddr(value.c_str());
					(check_and_cast<UDPErrorIndication*> (controlInfo))->setSrcAddr(srcAddr);
					return;
				}
			
				if (fieldName == "destAddr") {
					IPvXAddress destAddr(value.c_str());
					(check_and_cast<UDPErrorIndication*> (controlInfo))->setDestAddr(destAddr);
					return;
				}
				
				if (fieldName == "srcPort") {
					int srcPort = atoi(value.c_str());
					(check_and_cast<UDPErrorIndication*> (controlInfo))->setSrcPort(srcPort);
					return;
				}
				
				if (fieldName == "destPort") {
					int destPort = atoi(value.c_str());
					(check_and_cast<UDPErrorIndication*> (controlInfo))->setDestPort(destPort);
					return;
				}
			
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
				
			// TODO insert here other UDP control info
		
		}
		
		// handle TCP control info
		if (className.find("TCP") != std::string::npos) {
			
			// TCPCommand (base class)
			if (className == "TCPCommand"){
			
				if (fieldName == "connId") {
					(check_and_cast<TCPCommand*> (controlInfo))->setConnId(atoi(value.c_str()));
					return;
				}
		
				if (fieldName == "userId") {
					if (value == "random")
						value = generateRandomValue("int");
					(check_and_cast<TCPCommand*> (controlInfo))->setUserId(atoi(value.c_str()));
					return;
				}
				
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			
			}
			
			// TCPOpenCommand:TCPCommand
			if (className == "TCPSendCommand") {
				
				if (fieldName == "connId") {
					(check_and_cast<TCPCommand*> (controlInfo))->setConnId(atoi(value.c_str()));
					return;
				}
		
				if (fieldName == "userId") {
					(check_and_cast<TCPCommand*> (controlInfo))->setUserId(atoi(value.c_str()));
					return;
				}
				
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			
			}
			
			// TCPOpenCommand:TCPCommand
			if (className == "TCPOpenCommand") {
			
				if (fieldName == "connId") {
					(check_and_cast<TCPCommand*> (controlInfo))->setConnId(atoi(value.c_str()));
					return;
				}
		
				if (fieldName == "userId") {				
					(check_and_cast<TCPCommand*> (controlInfo))->setUserId(atoi(value.c_str()));
					return;
				}
			
				if (fieldName == "localAddr") { 				
					IPvXAddress localAddr(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setLocalAddr(localAddr);
					return;
				}
				
				if (fieldName == "remoteAddr") {				
					IPvXAddress remoteAddr(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setRemoteAddr(remoteAddr);
					return;
				}
				
				if (fieldName == "localPort") {
					int localPort = atoi(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setLocalPort(localPort);
					return;
				}
				
				if (fieldName == "remotePort") { 				
					int remotePort = atoi(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setRemotePort(remotePort);
					return;
				}
				
				if (fieldName == "fork") {
					bool fork = atoi(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setFork(fork);
					return;
				}

				if (fieldName == "dataTransferMode") {				
					int dataTransferMode = atoi(value.c_str());
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setDataTransferMode(dataTransferMode);
					return;
				}
				
				if (fieldName == "tcpAlgorithmClass") {
					(check_and_cast<TCPOpenCommand*> (controlInfo))->setTcpAlgorithmClass(value.c_str());
					return;
				}
			
				
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// TCPConnectInfo:TCPCommand
			if (className == "TCPConnectInfo") {
			
				if (fieldName == "connId") { 				
					(check_and_cast<TCPCommand*> (controlInfo))->setConnId(atoi(value.c_str()));
					return;
				}
		
				if (fieldName == "userId") {				
					(check_and_cast<TCPCommand*> (controlInfo))->setUserId(atoi(value.c_str()));
					return;
				}
			
				if (fieldName == "localAddr") {				
					IPvXAddress localAddr(value.c_str());
					(check_and_cast<TCPConnectInfo*> (controlInfo))->setLocalAddr(localAddr);
					return;
				}
				
				if (fieldName == "remoteAddr") {			
					IPvXAddress remoteAddr(value.c_str());
					(check_and_cast<TCPConnectInfo*> (controlInfo))->setRemoteAddr(remoteAddr);
					return;
				}
				
				if (fieldName == "localPort") {				
					int localPort = atoi(value.c_str());
					(check_and_cast<TCPConnectInfo*> (controlInfo))->setLocalPort(localPort);
					return;
				}
				
				if (fieldName == "remotePort") {				
					int remotePort = atoi(value.c_str());
					(check_and_cast<TCPConnectInfo*> (controlInfo))->setRemotePort(remotePort);
					return;
				}
				
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// TCPErrorInfo:TCPCommand
			if (className == "TCPErrorInfo") {
			
				if (fieldName == "connId") { 			
					(check_and_cast<TCPCommand*> (controlInfo))->setConnId(atoi(value.c_str()));
					return;
				}
		
				if (fieldName == "userId") {			
					(check_and_cast<TCPCommand*> (controlInfo))->setUserId(atoi(value.c_str()));
					return;
				}
			
				if (fieldName == "errorCode") {			
					int errorCode = atoi(value.c_str());
					(check_and_cast<TCPErrorInfo*> (controlInfo))->setErrorCode(errorCode);
					return;
				}
				
				if (fieldName == "messageText") {
					(check_and_cast<TCPErrorInfo*> (controlInfo))->setMessageText(value.c_str());
					return;
				}
				
				string errorMsg;
				errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, field ");
				errorMsg.append(fieldName);
				errorMsg.append(" not found");
				opp_error(errorMsg.c_str());
			}
			
			// TODO insert here other TCP commands
		}
		
		// handle IPv4ControlInfo
		if (className.find("IPv4") != std::string::npos) {
			if (className == "IPv4ControlInfo") {

				IPv4ControlInfo* ipv4ControlInfo = (check_and_cast<IPv4ControlInfo*>(controlInfo));

				if (fieldName == "destAddr") {			
					IPv4Address destAddr;
					// <A.S>
					if(isRandom)
                        destAddr =IPv4Address(stoul(value));
                    else
                        destAddr.set(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setDestAddr(destAddr);
					return;
				}
				
				if (fieldName == "srcAddr") {
					// <A.S>					
					IPv4Address srcAddr;
                    if(isRandom)
                        srcAddr =IPv4Address(stoul(value));
                    else
                        srcAddr.set(value.c_str());
                    
                                         
					IInterfaceTable *ift = InterfaceTableAccess().get();  
					InterfaceEntry* ie;
					IPv4Address registeredIPAddress;
					if (ift != NULL) {
						ie = ift->getInterfaceById(ipv4ControlInfo->getInterfaceId());
						if (ie != NULL) 
							registeredIPAddress = ie->ipv4Data()->getIPAddress();
						else {
							string errorMsg;
							errorMsg.assign("[void Change:: There is no interface with this interface ID. Please define the correct interface id.");
							opp_error(errorMsg.c_str());
						}
					}					
					
					
					if (srcAddr != registeredIPAddress) { //register the new IP in the secondary interface table
						InterfaceEntry* ieCopy = new InterfaceEntry(ie->getInterfaceModule());
						ieCopy->setInterfaceId(ie->getInterfaceId());
						IPv4InterfaceData *p = new IPv4InterfaceData();
						p->setIPAddress(srcAddr); //change the ip addr
						ieCopy->setIPv4Data(p);
						ift->addInterface(ieCopy);
					} 

					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setSrcAddr(srcAddr);
					return;
				}
			
				if (fieldName == "interfaceId") {			
					int interfaceId = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setInterfaceId(interfaceId);
					return;
				}
				
				if (fieldName == "multicastLoop") {
					bool multicastLoop = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setMulticastLoop(multicastLoop);
					return;
				}
				
				if (fieldName == "protocol") {			
					short protocol = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setProtocol(protocol);
					return;
				}

				if (fieldName == "typeOfService") {
					unsigned char typeOfService = *(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setTypeOfService(typeOfService);
					return;
				}

				if (fieldName == "timeToLive") {			
					short timeToLive = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setTimeToLive(timeToLive);
					return;
				}

				if (fieldName == "dontFragment") {
					bool dontFragment = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setDontFragment(dontFragment);
					return;
				}
				
				if (fieldName == "nextHopAddr") { 			
					IPv4Address nextHopAddr(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setNextHopAddr(nextHopAddr);
					return;
				}
				
				if (fieldName == "moreFragments") {
					bool moreFragments = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setMoreFragments(moreFragments);
					return;
				}
				
				// hex format, may contain spaces, hypens and colons
				if (fieldName == "macSrc") {
				    // <A.S>			
                    if (isRandom) {
				        (check_and_cast<IPv4ControlInfo*> (controlInfo))->setMacSrc(MACAddress(stoull(value)));
				    }
				    else {	
					    MACAddress macSrc(value.c_str());
					    (check_and_cast<IPv4ControlInfo*> (controlInfo))->setMacSrc(macSrc);
					}

					return;
				}
				
				if (fieldName == "macDest") { 
				    // <A.S>			
                    if (isRandom) {
				        (check_and_cast<IPv4ControlInfo*> (controlInfo))->setMacDest(MACAddress(stoull(value)));
				    }
				    else {	
					    MACAddress macDest(value.c_str());
					    (check_and_cast<IPv4ControlInfo*> (controlInfo))->setMacDest(macDest);
					}				
					
					return;
				}
				
				if (fieldName == "diffServCodePoint") {					
					int dscp = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setDiffServCodePoint(dscp);
					return;
				}
				
				if (fieldName == "explicitCongestionNotification") {
					int ecn = atoi(value.c_str());
					(check_and_cast<IPv4ControlInfo*> (controlInfo))->setExplicitCongestionNotification(ecn);
					return;
				}
			
			}
        
			if (className == "IPv4RoutingDecision") {
				// not supported yet
				string errorMsg = "[void Change::executeOnExternalInfo(cMessage**, string)] Error, IPv4RoutingDecision ControlInfo not supported";
				opp_error(errorMsg.c_str());
			}
				
			string errorMsg;
			errorMsg.assign("[void Change::executeOnExternalInfo(cMessage**, string)] Error, IPv4 field ");
			errorMsg.append(fieldName);
			errorMsg.append(" not found");
			opp_error(errorMsg.c_str());
		}
		
		// handle Ieee802
		if (className.find("Ieee802") != std::string::npos) {
		
			if (className == "Ieee802Ctrl") {
				
				if (fieldName == "src") {	
				    // <A.S>
				    if (isRandom) {
				        (check_and_cast<Ieee802Ctrl*> (controlInfo))->setSrc(MACAddress(stoull(value)));
				    }
				    else {	
					    MACAddress src(value.c_str());
					    (check_and_cast<Ieee802Ctrl*> (controlInfo))->setSrc(src);
					}
					return;
				}
				
				if (fieldName == "dest") {
				    // <A.S>
				    if (isRandom) {
				        (check_and_cast<Ieee802Ctrl*> (controlInfo))->setSrc(MACAddress(stoull(value)));
				    }
				    else {	
					    MACAddress dest(value.c_str());
					    (check_and_cast<Ieee802Ctrl*> (controlInfo))->setDest(dest);
					}
					return;
				}
				
				if (fieldName == "etherType") {			
					int etherType = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setEtherType(etherType);
					return;
				}
				
				if (fieldName == "interfaceId") {			
					int interfaceId = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setInterfaceId(interfaceId);
					return;
				}
				
				if (fieldName == "switchPort") {			
					if (value == "random")
						value = generateRandomValue("int");
					int switchPort = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setSwitchPort(switchPort);
					return;
				}
				
				if (fieldName == "ssap") {			
					int ssap = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setSsap(ssap);
					return;
				}
				
				if (fieldName == "dsap") {			
					int dsap = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setDsap(dsap);
					return;
				}
				
				if (fieldName == "pauseUnits") {			
					int pauseUnits = atoi(value.c_str());
					(check_and_cast<Ieee802Ctrl*> (controlInfo))->setPauseUnits(pauseUnits);
					return;
				}
				
			}
				
		}
		
		// TODO insert here control info of other protocols
		
		return;
	}
	
	// change sending fields	
	if (externalInfo == "sending") {
		if (fieldName == "outputGate") {
			bool hasParameter = (*packetToChange)->hasPar("outputGate");
			if (hasParameter == false) {
				(*packetToChange)->addPar("outputGate");
			}
			(*packetToChange)->par("outputGate").setStringValue(value.c_str());		
			return;
		}
		string errorMsg = "[void Change::executeOnExternalInfo(cMessage**, string)] Error, can't find the specified field in sending";
		opp_error(errorMsg.c_str());
	}

	string errorMsg = "[void Change::executeOnExternalInfo(cMessage**, string)] Error, can't recognize the specified external info";
	opp_error(errorMsg.c_str());
}


