/**
 * @file	UnconditionalAttack.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "UnconditionalAttack.h"
#include "seapputils.h"

#include "Drop.h"
#include "Clone.h"
#include "Change.h"
#include "Put.h"
#include "Retrieve.h"
#include "SEAPPExpression.h"
#include "Create.h"

UnconditionalAttack::UnconditionalAttack() : AttackBase (attack_t::UNCONDITIONAL)
{
	//EV << "UnconditionalAttack::UnconditionalAttack invoked" << endl;
}


UnconditionalAttack::~UnconditionalAttack()
{
	//EV << "UnconditionalAttack::~UnconditionalAttack invoked" << endl;
}


void UnconditionalAttack::setFrequency(double frequency)
{
    this->frequency = frequency;
}


double UnconditionalAttack::getFrequency() const
{	
	return frequency;
}


void UnconditionalAttack::addAction(ActionBase* action)
{
	actions.push_back(action);
}


void UnconditionalAttack::execute(vector<cMessage*> &putMessages)
{
	//EV << "UnconditionalAttack::execute invoked" << endl;

	map<string,int>  packetTable;
	vector<cMessage*> generatedMessages;

	cMessage* generatedMessage = nullptr;
	cPacket* generatedPacket = nullptr;
	cMessage** targetMessage = nullptr;

	string packetName;

	int packetPosition;

	// execute all actions composing the attack
	for (size_t i = 0; i < actions.size(); i++) {
	 
		generatedMessage = nullptr;
		generatedPacket = nullptr;

		switch (actions[i]->getActionType()) {
			// Drop
			case action_t::DROP: {
				
				Drop* drop = (Drop*) actions[i];

				packetName = drop->getPacketName();
				packetPosition = packetTable[packetName];

				// process
				targetMessage = &generatedMessages[packetPosition];

				drop->execute(targetMessage);
			
				break;
			
			}
			
			// Clone
			case action_t::CLONE: {
			
				Clone* clone = (Clone*) actions[i];

				string clonePacketName = clone->getClonePacketName();
				packetName = clone->getPacketName();
				packetPosition = packetTable[packetName];
				
				// process an already existing packet
				targetMessage = &generatedMessages[packetPosition]; 

				clone->execute(&generatedMessage, *targetMessage);

				// add the cloned packet to the vector generatedMessages
				generatedMessages.push_back(((cMessage*)generatedMessage));

				// update the map packetTable
				packetTable[clonePacketName] = generatedMessages.size() - 1;
			
				break;				
			}
			
			// Change
			case action_t::CHANGE: {
			
				Change* change = (Change*) actions[i];

				bool isRandomValue;
				string valueName;
				string value;

				// retrieve the valueName (i.e. the variable name)
				valueName = change->getValue();
				isRandomValue = (valueName == "RANDOM");
				
				// compute a random value
				if (isRandomValue) {
					double maxValue;
					double minValue;
					double randomValue;
					maxValue = double( *(variableTable["MAX"]) );
					minValue = double( *(variableTable["MIN"]) );
					randomValue = minValue + (dblrand() * (maxValue - minValue));
					value.assign(to_string(randomValue));
				} 
				else{
					// retrieve the value from a stored variable
					value = variableTable[valueName]->getValue();
				}
				
				packetName = change->getPacketName();
				packetPosition = packetTable[packetName];
				
				// process an already existing packet
				targetMessage = &generatedMessages[packetPosition];

				change->execute(targetMessage, value);
			
				break;
			}

			// Put
			case action_t::PUT: {
			
				Put* put = (Put*) actions[i];

				packetName = put->getPacketName();
				packetPosition = packetTable[packetName];

				// process an already existing packet
				targetMessage = &generatedMessages[packetPosition];

				generatedMessage = put->execute(*targetMessage);

				// add the PutMsg to put_messages
				putMessages.push_back(generatedMessage);
			
				break;
			}

			// Retrieve
			case action_t::RETRIEVE: {
			
				Variable* variable;
			  	string variableName;
			  	
				Retrieve* retrieve = (Retrieve*) actions[i];

				packetName = retrieve->getPacketName();
				packetPosition = packetTable[packetName];
				
				// process an already existing packet
				targetMessage = &generatedMessages[packetPosition]; 
				variable = retrieve->execute(*targetMessage);
	
				// delete the old value and update the variable table
				variableName.assign(retrieve->getVariableName());
				delete variableTable[variableName];
				variableTable[variableName] = variable;

				break;
			}

			// Expression
			case action_t::EXPRESSION: {
				SEAPPExpression* expression = (SEAPPExpression*) actions[i];
				expression->execute(&variableTable, &variableStack);
				
				break;
			}

			// Create
			case action_t::CREATE: {
				Create* create = (Create*)actions[i];
				
				// Retrieve the name of the packet to be created
				string generatedPacketName = create->getPacketName();

				create->execute(&generatedPacket);

				// Add the new packet to the vector 'new_messages'
				generatedMessages.push_back(generatedPacket);

				// Update the 'packetTable' map
				packetTable[generatedPacketName] = generatedMessages.size() - 1;
				
				break;
								
			}

			// Send not recognized by unconditional attacks			
			case action_t::SEND: {
				opp_error("[void UnconditionalAttack::execute(vector<cMessage*> &)] can't recognize the Send action, check the Attack Configuration File");
				return; // never executed
			}
			
			// Destroy not recognized by unconditional attacks			
			case action_t::DESTROY: {
				opp_error("[void UnconditionalAttack::execute(vector<cMessage*> &)] can't recognize the Destroy action, check the Attack Configuration File");
				return; // never executed
			}

			// Move not recognized by unconditional attacks			
			case action_t::MOVE: {
				opp_error("[void UnconditionalAttack::execute(vector<cMessage*> &)] can't recognize the Move action, check the Attack Configuration File");
				return; // never executed
			}

			// Fakeread not recognized by unconditional attacks			
			case action_t::FAKEREAD: {
				opp_error("[void UnconditionalAttack::execute(vector<cMessage*> &)] can't recognize the Fakeread action, which is not supported by seapp");
				return; // never executed
			}

		}
 
	}

}





















