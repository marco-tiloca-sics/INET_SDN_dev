/**
 * @file	ConditionalAttack.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "ConditionalAttack.h"
#include "seapputils.h"

#include "Drop.h"
#include "Clone.h"
#include "Change.h"
#include "Send.h"
#include "Put.h"
#include "Retrieve.h"
#include "SEAPPExpression.h"
#include "Create.h"


ConditionalAttack::ConditionalAttack() : AttackBase(attack_t::CONDITIONAL)
{
	minimumInvolvedLayer = NONE_LAYER;
    packetFilter = nullptr;

	EV << "ConditionalAttack::ConditionalAttack invoked" << endl;
}


ConditionalAttack::~ConditionalAttack()
{
	EV << "ConditionalAttack::~ConditionalAttack invoked" << endl;
    delete packetFilter;
}


void ConditionalAttack::addAction(ActionBase* action)
{
    // append the action to the set of actions composing the attack
	actions.push_back(action);
    // update the minimum involved layer if necessary
	int actionLayer = action->getInvolvedLayer();
	if (minimumInvolvedLayer > actionLayer) {
		minimumInvolvedLayer = actionLayer;
	}
}


void ConditionalAttack::setPacketFilter(string monolithicPacketFilter)
{    
    // delete the old packet-filter if exists
    if (packetFilter != nullptr) {
        delete packetFilter;
    }
    packetFilter = new PacketFilter(monolithicPacketFilter);
}


bool ConditionalAttack::matchPacketFilter(cMessage* packet) const
{
    // check if the current layer contains all the informations
	int packetLayer = getPacketLayer((cPacket*)packet);
    if (packetLayer > minimumInvolvedLayer) {
        return false;
    }

    return packetFilter->matchPacketFilter(packet);
}


void ConditionalAttack::execute(cMessage** packet, vector<cMessage*> &generatedPackets, vector<double> &delays, double &delay)
{
	EV << "ConditionalAttack::execute invoked" << endl;

	map<string,int> packetTable;
	packetTable["original"] = -1;
	
	cMessage* generatedPacket = nullptr;
	
	cMessage** targetPacket = nullptr;
	
	string packetName;
	int packetPosition;
	
	// set isFiltered parameter (it exists and its value is false) to avoid multiple execution of the attack by the same node on different layer 
	setParameterRecursively(*packet, "isFiltered", true);
	
	// execute all actions composing the attack
	for (size_t i = 0; i < actions.size(); i++) {
		
		generatedPacket = nullptr;
	
		switch (actions[i]->getActionType()) {
	
			case action_t::DROP: {
				Drop* drop = (Drop*) actions[i];
				packetName = drop->getPacketName();
				packetPosition = packetTable[packetName];
				
				// process the original intercepted packet
				if (packetPosition == -1) {
					targetPacket = packet;
				}
				// process another existing packet
				else {
					targetPacket = &generatedPackets[packetPosition];
				}
				drop->execute(targetPacket);
				break;
			}
			
			case action_t::CLONE: {
				Clone* clone = (Clone*) actions[i];
			
				// retrieve the names of the clone packet and the original packet
				string clonePacketName = clone->getClonePacketName();
				packetName = clone->getPacketName();
				packetPosition = packetTable[packetName];

				// process the original intercepted packet
				if (packetPosition == -1) {
					targetPacket = packet;
				}
				// process another existing packet
				else {
					targetPacket = &generatedPackets[packetPosition];
				}
				
				clone->execute(&generatedPacket, *targetPacket);
				
				// add the new packet to the generatedPackets
				generatedPackets.push_back(((cMessage*)(generatedPacket)));

				// add a zero delivery delay to the new packet
				delays.push_back(0.0);

				// update the packetTable map
				packetTable[clonePacketName] = generatedPackets.size()-1;
				
				break;

			}
			
			case action_t::CHANGE: {
				Change* change = (Change*) actions[i];			
				
				packetName = change->getPacketName();
				packetPosition = packetTable[packetName];
				
				bool isRandomValue;
				string valueName;
				string value;

				valueName = change->getValue();
				isRandomValue = (valueName == "RANDOM");		
				
				// process the original intercepted packet
				if (packetPosition == -1) {
					targetPacket = packet;
				}
				// process another existing packet
				else {
					targetPacket = &generatedPackets[packetPosition];
				}
				
				if (isRandomValue) {
					double randomValue;
					double maxValue;
					double minValue;
					
					minValue = double( *(variableTable["MIN"]) );
					maxValue = double( *(variableTable["MAX"]) );
					
					randomValue = minValue + ( dblrand() * (maxValue - minValue) );
					
					value = to_string(randomValue); 
				}
				else {
					value = variableTable[valueName]->getValue();
				}
				
				change->execute(targetPacket, value);
				break;
			}
			
			case action_t::SEND: {
				Send* send = (Send*) actions[i];

				packetName = send->getPacketName();
				packetPosition = packetTable[packetName];

				// in case of original packet retrieve only the delay
				if (packetPosition == -1){
					delay = send->getSendDelay();
				}
				// process another existing packet
				else{
					targetPacket = &generatedPackets[packetPosition];
					send->execute(*targetPacket);
					delays[packetPosition] = send->getSendDelay();
				}
				break;			
			}
	
			case action_t::PUT: {
				Put* put = (Put*) actions[i];
			
				packetName = put->getPacketName();
				packetPosition = packetTable[packetName];

				// process the original intercepted packet
				if (packetPosition == -1){
					targetPacket = packet;
				}
				// process another existing packet
				else {
					targetPacket = &generatedPackets[packetPosition];
				}
				
				generatedPacket = put->execute(*targetPacket);
				generatedPackets.push_back(generatedPacket);
				delays.push_back(0.0);
				
				break;
			}
	
			case action_t::RETRIEVE: {
				Variable* variable;
				string variableName;
				Retrieve* retrieve = (Retrieve*) actions[i];

				packetName = retrieve->getPacketName();
				packetPosition = packetTable[packetName];
				
				// process the original intercepted packet
				if (packetPosition == -1) {
					targetPacket = packet;
				}
				// process another existing packet
				else{
					targetPacket = &generatedPackets[packetPosition];
				}
				
				variable = retrieve->execute(*targetPacket);
				
				// delete the old value and update the variable table
				variableName.assign(retrieve->getVariableName());
				delete variableTable[variableName];
				variableTable[variableName] = variable;
				
				break;
			}
			
			case action_t::EXPRESSION: {
				SEAPPExpression* expression = (SEAPPExpression*) actions[i];
				expression->execute(&variableTable, &variableStack);
				break;
			}
			
			case action_t::CREATE: {
			
				cPacket* createdPacket = nullptr;
				Create* create = (Create*) actions[i];
				
				string createdPacketName = create->getPacketName();
					
				create->execute(&createdPacket);
	
				generatedPackets.push_back(((cMessage*)(createdPacket)));
				delays.push_back(0.0);
				packetTable[createdPacketName] = generatedPackets.size()-1;
				
				break;
			}
	
		}
		
	}

}
