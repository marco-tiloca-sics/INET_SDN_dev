/**
 * @file CompoundBlock.cc
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "CompoundBlock.h"
#include "seapputils.h"
#include "stdlib.h"


CompoundBlock::CompoundBlock(vector<string> blockElements) : FilterBlock(block_t::COMPOUND, blockElements)
{
}


CompoundBlock::~CompoundBlock() 
{
}


int CompoundBlock::getLayer() const
{
    // get the layer's name
    vector<string> fullPath = tokenize(blockElements[2], '.');
    return layertoi(fullPath[0]);
}


bool CompoundBlock::solveFilterBlock(cMessage* msg) const
{
    // tokenize the full path to access to names of layer and fields
    vector<string> fullPath = tokenize(blockElements[2], '.');    
    
    // retrieve the encapsulated message belonging to the block's layer
    int blockLayer = layertoi(fullPath[0]);
    int msgLayer = getPacketLayer((cPacket*)msg);
    cMessage* encapsulatedMsg = msg;
    while (msgLayer < blockLayer) {
        encapsulatedMsg = (cMessage*)(((cPacket*)encapsulatedMsg)->getEncapsulatedPacket());
        if (encapsulatedMsg == nullptr) {
            return false;
        }
        msgLayer = getPacketLayer((cPacket*)encapsulatedMsg);
    }
    
    // retrieve the path of fields
    vector<string> pathOfFields;
    for (size_t i = 1; i < fullPath.size(); i++) {
        pathOfFields.push_back(fullPath[i]);
    }
    
    // try to follow the path of fields until the last field is reached
    cClassDescriptor* descriptor = cClassDescriptor::getDescriptorFor(encapsulatedMsg);
    void* compoundField = encapsulatedMsg;
    int fieldIndex = -1;
    for (size_t i = 0; i < pathOfFields.size(); i++) {

        // the first entry is packet's top level field
        if (i==0) {
            fieldIndex = descriptor->findField(encapsulatedMsg, pathOfFields[i].c_str());
        }
        else {
            fieldIndex = descriptor->findField(compoundField, pathOfFields[i].c_str());
        }
        
        // field not found in the current packet
        if (fieldIndex==-1) {
            return false;
        }
        
        // all entries (except the last) refer compound fields
        if (i != pathOfFields.size()-1) {
            string structName;
            // get the pointer-to the compound field
            if (i==0) {
                structName = descriptor->getFieldStructName(encapsulatedMsg, fieldIndex);
                compoundField = descriptor->getFieldStructPointer(encapsulatedMsg, fieldIndex, 0);
            }
            else {
                structName = descriptor->getFieldStructName(compoundField, fieldIndex);
                compoundField = descriptor->getFieldStructPointer(compoundField, fieldIndex, 0);
            }
            
            // get the descriptor of the compound field
            descriptor = cClassDescriptor::getDescriptorFor(structName.c_str());
        }
        
    }
    // use the descriptor to get the last field's content
    string actualValue = descriptor->getFieldAsString((cObject*)compoundField, fieldIndex, 0);

    // retrieve the arithmetic operator from the inner block
    string arithmeticOperator = blockElements[3];
    // retrieve the operand from the inner block
    string operand = blockElements[4];
    // perform the arithmetic operation (i.e. solve the inner block)
    double actualResult = evaluateArithmetic(stod(actualValue.c_str()), stod(operand.c_str()), arithmeticOperator);

    // retrieve the comparison operator from the block
    string comparisonOperator = blockElements[6];
    // retrieve the accepted values for the comparison operation from the outer block
    vector<string> acceptedValues = tokenize(blockElements[7], '|');
    // perform the comparison operation (i.e. solve the outer block)
    bool comparisonResult = false;
    for (size_t i = 0; i < acceptedValues.size(); i++) {
        comparisonResult = evaluate(actualResult, stod(acceptedValues[i]), comparisonOperator );
        if (comparisonResult == true) {
            return true;
        } 
    }

    return false;
    
}
