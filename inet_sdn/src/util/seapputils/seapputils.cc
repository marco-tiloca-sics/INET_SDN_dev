/**
 * @file	seapputils.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */
 
 
#include "seapputils.h"
#include <sstream>
#include <map>
#include <random>
#include <cstring>

void tokenize(vector<string>& tokens, const string str, const char delim)
{
	string word;
	stringstream stream(str);
	while (getline(stream, word, delim)) {
		tokens.push_back(word);
	}
}


vector<string> tokenize(const string str, const char delim)
{    
    vector<string> tokens;
    string word;
    stringstream stream(str);
    // tokenize str upon each occurrence of delim 
    while (getline(stream, word, delim)) {
        tokens.push_back(word);
    }

    return tokens;
}


cPacket* getDeepestEncapsulatedPacket(cPacket* packet)
{
	cPacket* encapsulatedPacket = packet;
	while (encapsulatedPacket != nullptr) {
		encapsulatedPacket = encapsulatedPacket->getEncapsulatedPacket();
	}
	return encapsulatedPacket;
}


int getPacketLayer(cPacket* packet)
{
	string packetClassName = packet->getClassName();
	//std::cout<<"seaputils: packet class name = " << packetClassName <<endl;
	// add other classnames here

	if (packetClassName == "cPacket") {
		return 5;	
    }
    	
	if (packetClassName == "TrafficLightPacket") {
		return 5;
	}	
	if (packetClassName == "TrafficLightStatus") {
		return 5;
	}
	
	if (packetClassName == "TrafficLightCmd") {
		return 5;
	}
	
	// <A.S>
	if (packetClassName == "ApplicationPacket") {
		return 5;
	}
	// <A.S>
	if (packetClassName == "SendApplicationPacket") {
		return 5;
	}
	
	// <A.S>
	if (packetClassName == "MeasurementData") {
	    return 5;
	}
	
	// <A.S> sdn
	if (packetClassName == "OFP_Flow_Stats_Request") {
		return 5;
	}
	if (packetClassName == "OFP_Flow_Stats_Reply") {
		return 5;
	}
	if (packetClassName == "OFP_Flow_Removed") {
		return 5;
	}
	if (packetClassName == "OFP_Packet_In") {
		return 5;
	}
    if (packetClassName == "OFP_Packet_Out") {
		return 5;
	}
	if (packetClassName == "OFP_Flow_Mod") {
		return 5;
	}
	
	if (packetClassName == "UDPPacket") {
		return 4;
	}
    
    if (packetClassName == "TCPPacket") {
		return 4;
	}
	
	// <A.S> sdn
	if (packetClassName == "TCPSegment") {
	    return 4;
	}
	
	if (packetClassName == "IPv4Datagram") {
		return 3;
	}
	if (packetClassName == "ARPPacket") {
		return 2;
	}	
	if (packetClassName == "PPPFrame") {
		return 2;
	}
	if (packetClassName == "EthernetIIFrame") {
		return 2;
	}
	
    opp_error("[int getPacketLayer(cPacket*)] Error, packetClassName not recognized");
	
}


int layertoi(const string layer)
{
    if (layer == "APP") {
		return 5;
    }
    else {
        
        if (layer == "TRA") {
            return 4;
        }
        else {
           
            if (layer == "NET") {
                return 3;
            }
            else {
                if (layer == "MAC") {
                    return 2;
                } 
                // <A.S>
                else {
				  if (isControlInfo(layer)) 
					return 0; // special number representing control info
				  
				  else {
					string errorMsg = ("[int layertoi(const string)] Error, layer ");
					errorMsg.append(layer);
					errorMsg.append(" not recognized; it supports only APP, TRA, NET or MAC or control info.");
					opp_error(errorMsg.c_str());
				  }
                }
            }
        }
    }
}



void setParameterRecursively(cMessage* msg, const string parameterName, const bool parameterValue) 
{
	bool hasPar = false;
	cMessage* encapsulatedMsg = msg;
	
	while (encapsulatedMsg != nullptr) {
		hasPar = encapsulatedMsg->hasPar(parameterName.c_str());
		
        if (hasPar) {
			encapsulatedMsg->par(parameterName.c_str()).setBoolValue(parameterValue);
			encapsulatedMsg = (cMessage*) ((cPacket*) encapsulatedMsg)->getEncapsulatedPacket();		
		}
		else {
			string errorMsg;
			errorMsg.append("[void setParameterRecursively(cMessage*, const string, const bool)] Error, parameter '");
			errorMsg.append(parameterName);
			errorMsg.append("' not found");
            opp_error(errorMsg.c_str());	
		}
	}
}



string getValueType(string value) {
    vector<string> tokens;
	tokenize(tokens, value, '_');
	return tokens[1];
}

cPacket* hardCopy (cPacket* packetToCopy)
{
	cPacket* layer2Packet = nullptr;
	cPacket* layer3Packet = nullptr;
	cPacket* layer4Packet = nullptr;
	cPacket* layer5Packet = nullptr;

	if (packetToCopy == nullptr) {
		return nullptr;
	}

	// controlInfo is present only in the outer packet
	cObject* controlInfo = NULL;
	if (packetToCopy->getControlInfo()!=NULL)
	    controlInfo = (packetToCopy->getControlInfo())->dup();
	    
	int packetLayer = getPacketLayer(packetToCopy);

	switch (packetLayer) {
	
		case 2: {
			layer2Packet = packetToCopy->dup();	
			if (controlInfo != NULL)
			    layer2Packet->setControlInfo(controlInfo);
			layer3Packet = layer2Packet->decapsulate();
			if (layer3Packet == nullptr){
				return layer2Packet;
			}
		}
	
		case 3: {
			if (layer3Packet == nullptr) {
				// is outer packet, append control info
				layer3Packet = packetToCopy->dup();
				if (controlInfo != NULL)
				    layer3Packet->setControlInfo(controlInfo);
			}
			layer4Packet = layer3Packet->decapsulate();	
			
			if (layer4Packet == nullptr) {
				if (layer2Packet != nullptr) {
					layer2Packet->encapsulate(layer3Packet);
					return layer2Packet;
				}
				else{
					return layer3Packet;
				}
			}
		}
	
		case 4: {
			if (layer4Packet == nullptr) {
				// is outer packet, append control info			
				layer4Packet = packetToCopy->dup();
				if (controlInfo != NULL)
				    layer4Packet->setControlInfo(controlInfo);
			}
			layer5Packet = layer4Packet->decapsulate();
			if (layer5Packet == nullptr) {	
				if (layer3Packet != nullptr) {
					layer3Packet->encapsulate(layer4Packet);
					if (layer2Packet != nullptr) {
						layer2Packet->encapsulate(layer3Packet);
						return layer2Packet;
					}
					else{
						return layer3Packet;
					}
				}
				else{
					return layer4Packet;
				}
			}
		}

		case 5: {
			if (layer5Packet == nullptr) {
				// is outer packet, append control info
				layer5Packet = packetToCopy->dup();
				if (controlInfo != NULL)
				    layer5Packet->setControlInfo(controlInfo);
			}
			if (layer4Packet != nullptr) {
				layer4Packet->encapsulate(layer5Packet);
				if (layer3Packet != nullptr) {
					layer3Packet->encapsulate(layer4Packet);
					if (layer2Packet != nullptr) {
						layer2Packet->encapsulate(layer3Packet);
						return layer2Packet;
					}
					else{
						return layer3Packet;
					}
				}
				else{
					return layer4Packet;
				}
			}
			else{
				return layer5Packet;
			}

		}	
		
	}
}

// <A.S>
bool isControlInfo(const string layer) {
	if ((layer == "attackInfo") || (layer == "controlInfo")) 
		return true;
	return false;
} 

// <A.S>
bool hasPayload(cMessage* msg) {
	bool isPacket = msg->isPacket();
	if (isPacket) { 
		cPacket *packet = dynamic_cast<cPacket*>(msg);
		cPacket* encapsulatedPacket = packet->getEncapsulatedMsg();
		if (encapsulatedPacket != nullptr) 
			return true;
		return false;
	}
	return false;
}

// <A.S>
bool getParamFromEncapsulatedPacket(cMessage* msg, const string parameterName) {
	bool isPacket = msg->isPacket();
	if (isPacket) { 
		cPacket *packet = dynamic_cast<cPacket*>(msg);
		cPacket* encapsulatedPacket = packet->getEncapsulatedMsg();
		if (encapsulatedPacket != nullptr) {
			if (encapsulatedPacket->hasPar(parameterName.c_str())) 
				return encapsulatedPacket->par(parameterName.c_str()).boolValue();
		}
		else {
			string errorMsg;
			errorMsg.append("[bool getParamFromEncapsulatedPacket(cMessage* msg, sting parameterName)] Error, there is no '");
			errorMsg.append("' encapsulated packet");
            opp_error(errorMsg.c_str());	
		}
	}	
}

// <A.S>
bool isRandomValue(string value) {
    if (value.find("RANDOM") != string::npos)
        return true;
    return false;
}

// <A.S>
int generateRandomIntValue(int a, int b) {
    random_device rand_dev; //uniformly-distributed integer random number generator that produces non-deterministic random numbers
	mt19937 generator(rand_dev()); //random number engine based on Mersenne Twister algorithm. It satisfies the UniformRandomBitGenerator.
								   //It produces high quality unsigned integer random numbers of type UIntType on the interval [0, 2w-1]
	uniform_int_distribution<int> distr(a, b);
    return distr(generator);
}

// <A.S>
string generateRandomValue(string networkAddress, string netmask) {
    std::vector<string> netAddrTokens = tokenize(networkAddress, '.');
    std::vector<string> netmaskTokens = tokenize(netmask, '.');
    
    uint32 netAddr = (stoi(netAddrTokens[0]) << 24) + (stoi(netAddrTokens[1]) << 16) + (stoi(netAddrTokens[2]) << 8) + stoi(netAddrTokens[3]);
    uint32 mask = (stoi(netmaskTokens[0]) << 24) + (stoi(netmaskTokens[1]) << 16) + (stoi(netmaskTokens[2]) << 8) + stoi(netmaskTokens[3]);

    uint32 range = ~mask;
    
    random_device rand_dev; 
	mt19937 generator(rand_dev()); 
    uniform_int_distribution<uint> distr(1, range-1);
    
    uint32 hostid = distr(generator);
    uint32 addr = netAddr + hostid;

    return to_string(addr);
}


// <A.S>
string generateRandomValue(const char *fieldType) {
	string field (fieldType);
	
	random_device rand_dev; //uniformly-distributed integer random number generator that produces non-deterministic random numbers
	mt19937 generator(rand_dev()); //random number engine based on Mersenne Twister algorithm. It satisfies the UniformRandomBitGenerator.
								   //It produces high quality unsigned integer random numbers of type UIntType on the interval [0, 2w-1]. 
								   
	if (field.find("IP") != string::npos) {		
		uniform_int_distribution<uint> distr(0, 255); //Produces random integer values i, uniformly distributed on the closed interval [a, b]
		uint32 addr = (distr(generator) << 24) + (distr(generator) << 16) + (distr(generator) << 8) + distr(generator);
		return to_string(addr);
	}
	else if (field.find("MAC") != string::npos) {
	    uniform_int_distribution<uint> distr(0, 255); //Produces random integer values i, uniformly distributed on the closed interval [a, b]		
        uint64 addr = 0x0AAA00000000;
        addr += ((distr(generator)) << 24) + ((distr(generator) ) << 16) + ((distr(generator)) << 8) + distr(generator);
		return to_string(addr);
	}
	else if (strcasecmp(fieldType, "int") == 0) {
		uniform_int_distribution<int> distr(0, INT_MAX-1);
		string randomNumber = to_string(distr(generator));
		return randomNumber;
	}
	else if (strcasecmp(fieldType, "short") == 0) {
		uniform_int_distribution<short> distr(0, SHRT_MAX-1);
		string randomNumber = to_string(distr(generator));
		return randomNumber;
	}
	else {
		string errorMsg;
		errorMsg.append("[string generateRandomValue(const char *fieldType) ] Unsupported type of field.");
		opp_error(errorMsg.c_str());
	}
}
