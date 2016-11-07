/**
 * @file	GlobalFilter.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "GlobalFilter.h"
#include "UnconditionalAttack.h"
#include "PutMessages.h"
#include "Parser.h"
#include "seapputils.h"
#include "omnetpp.h"
#include <string>


Define_Module(GlobalFilter);


void GlobalFilter::initializeAttacks()
{
	cModule* node = this;
	Parser* parser = new Parser(node);
	parser->parseConfigurationFile(attack_t::UNCONDITIONAL, unconditionalAttacks);

	// schedule selfMessages in order to trigger the unconditional attacks
	string index;
	string selfMessageName;

	for (size_t i = 0; i < unconditionalAttacks.size(); i++) {
		index.assign(to_string(i));
		selfMessageName.assign("Fire UnconditionalAttack[");
		selfMessageName.append(index);
		selfMessageName.append("]");
		cMessage* selfMessage = new cMessage(selfMessageName.c_str(), (short) attack_t::UNCONDITIONAL);
		scheduleAt(unconditionalAttacks[i]->getOccurrenceTime(), selfMessage);
	}


	// delete parser
	delete parser;
}


void GlobalFilter::initialize()
{
	initializeAttacks();
}


void GlobalFilter::handleMessage(cMessage* msg)
{
	bool isSelfMessage = msg->isSelfMessage();

	if (isSelfMessage) {
		string msgName = msg->getName();
		
		// handle unconditional attack
		if (msgName.find("Unconditional") != std::string::npos) {
			
			UnconditionalAttack* unconditionalAttack = nullptr;
			vector<cMessage*> generatedMessages;
			int index;	
			double frequency;
			double scheduleTime;
			vector<string> tokens;
			
			// get unconditional attack index			
			tokenize(tokens, msgName, '[');
			tokens[1].pop_back();
			index = atoi(tokens[1].c_str());
		
			// execute the unconditional attack
			unconditionalAttack = (UnconditionalAttack*) (unconditionalAttacks[index]->getAttack());
			unconditionalAttack->execute(generatedMessages);
		
			// deliver all the created put messages
			for (size_t i = 0; i < generatedMessages.size(); i++){
				handlePutMessage(generatedMessages[i]);
			}	
		
			// re-schedule the attack if not-zero frequency
			frequency = unconditionalAttack->getFrequency();
			if (frequency == 0){
				delete msg;
				return;			
			}
			else {
				scheduleTime = frequency + (simTime().dbl());
				cMessage* selfMessage = new cMessage( msgName.c_str(), (short) attack_t::UNCONDITIONAL);
				scheduleAt( scheduleTime, selfMessage );				
			}
			
		}
		
	}
	else {
		// handle messages from another node in the network, i.e. a PutMsg	
		if (isPutMsg(msg)) {
			handlePutMessage(msg);
		}
		else {
			EV << "[void GlobalFilter::handleMessage(cMessage*)] GlobalFilter can handle only PutMsg or selfMsg messages" << endl;
		}
		
	}
	
	delete msg;
}


void GlobalFilter::handlePutMessage(const cMessage* msg)
{
	cMessage* encapsulatedPacket = nullptr;
	PutMsg* putMsg = nullptr;
	vector<int> recipientNodes;
	direction_t direction;
	double forwardingDelay;
	bool isStatUpdated;
	
	putMsg = (PutMsg*)(msg);
	recipientNodes = putMsg->getRecipientNodes();
	direction = putMsg->getDirection();
	
	isStatUpdated = putMsg->getStatUpdated();
	forwardingDelay = putMsg->getForwardingDelay();
	
	encapsulatedPacket = putMsg->getMsg();

	
	int gateSize = this->gateSize("nodes$o");
	cGate* gate = nullptr;
	cGate* nextGate = nullptr;
	cModule* nextGateOwner = nullptr;
	for (size_t i = 0; i < recipientNodes.size(); i++) {
		
		// scan the nodes vector gate to find the current recipient node
		for (int j = 0; j < gateSize; j++) {
			gate = this->gate("nodes$o", j);
			nextGate = gate->getNextGate();
			nextGateOwner = check_and_cast<cModule*>(nextGate->getOwner());
			
			if (recipientNodes[i] == nextGateOwner->getId()) {
				PutReq* putReq = new PutReq(encapsulatedPacket, direction, isStatUpdated);
				sendDelayed(putReq, forwardingDelay, "nodes$o", j);
				break;
			}
		
		}

	}
	// <A.S>
    delete msg;
}


void GlobalFilter::finishSpecific()
{
}


GlobalFilter::GlobalFilter()
{

}


GlobalFilter::~GlobalFilter()
{
}


