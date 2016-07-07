/**
 * @file	Retrieve.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "Retrieve.h"
#include "seapputils.h"


Retrieve::Retrieve(const string fields, const string variableName) : ActionBase(action_t::RETRIEVE)
{
	// fields has the pattern "layer.field"
	vector<string> tokens;
	tokenize(tokens, fields, '.');

	involvedLayer = layertoi(tokens[0]);

	this->fieldName = tokens[1];
	this->variableName = variableName;	
}


Retrieve::~Retrieve()
{  
}


string Retrieve::getFieldName() const
{
	return fieldName;
}


string Retrieve::getVariableName() const
{
	return variableName;
}


Variable* Retrieve::execute(cMessage* msg) const
{
	cClassDescriptor* descriptor; 
	int fieldIndex;
	string value;
	
	cMessage* substituteMsg = nullptr;
	cMessage* encapsulatedMsg = nullptr;

	// work on a clone of the original packet
	substituteMsg = (cMessage*) hardCopy((cPacket*)msg);
	encapsulatedMsg = substituteMsg;
	
	int packetLayer = getPacketLayer((cPacket*) encapsulatedMsg);

	// retrieve the encapsulated packet belonging to the right layer
	while (packetLayer < involvedLayer) {
	
		encapsulatedMsg = (cMessage*) ((cPacket*) encapsulatedMsg)->getEncapsulatedPacket();
		
		if(encapsulatedMsg == nullptr){
			string errorMsg = "[Variable* Retrieve::execute(cMessage*)] Error, tryed to access in a layer not contained in this packet, check your packet filter";
			opp_error(errorMsg.c_str());
		}
		
		packetLayer = getPacketLayer((cPacket*) encapsulatedMsg);
	}
	
	descriptor = cClassDescriptor::getDescriptorFor(encapsulatedMsg);
	fieldIndex = descriptor->findField(encapsulatedMsg, fieldName.c_str());
	
	// fieldName doesn't exist
	if (fieldIndex == -1) {
		string errorMsg = "[Variable* Retrieve::Execute(cMessage*)] Error, can't find ";
		errorMsg.append(fieldName);
		errorMsg.append(", the field doesn't exist");
		opp_error(errorMsg.c_str());
	}
	
	// retrieve the value as a string
	value = descriptor->getFieldAsString(encapsulatedMsg, fieldIndex, 0);

	return new Variable(value, get_variable_format(value));
}
