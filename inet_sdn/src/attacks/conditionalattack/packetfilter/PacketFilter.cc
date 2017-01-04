/**
 * @file PacketFilter.cc
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "PacketFilter.h"
#include "ElementaryBlock.h"
#include "CompoundBlock.h"
#include "seapputils.h"
#include <stdexcept>
#include <list>


PacketFilter::PacketFilter(string monolithicPacketFilter)
{
    // split monolithicPacketFilter in single elements
    vector<string> elements = tokenize(monolithicPacketFilter, ':');
    
    // extract logical operators in reverse order
    while (!elements.empty()) {
        if ((elements.back() == "AND") || (elements.back() == "OR")) {
            filterOperators.push_back(elements.back());
            elements.pop_back();        
        }
        else {
            break;
        }
    }
    
    // retrieve blocks (access elements in safe mode)
    while (!elements.empty()) {
        
        // iterators
        vector<string>::iterator front = elements.begin();
        vector<string>::iterator back;
        try {
            // check if the block in the head is an elementary block
            if (elements.at(ElementaryBlock::elementaryBlockSize-1) == "]") {
                // set iterator on the last element of the block
                back = front + ElementaryBlock::elementaryBlockSize;
                // extract the block
                vector<string> block(front, back);
                elements.erase(front, back);
                // store the pointer-to the elementary block
                filterBlocks.push_back(new ElementaryBlock(block));
                continue;
            }
            // check if the block in the head is a compound block
            if (elements.at(CompoundBlock::compoundBlockSize-1) == "]") {
                // set iterator on the last element of the block
                back = front + CompoundBlock::compoundBlockSize;
                // extract the block
                vector<string> block(front, back);
                elements.erase(front, back);
                // store the pointer-to the compound block
                filterBlocks.push_back(new CompoundBlock(block));
                continue;
            }
            
            // packet-filter has a bad structure in the xml
            opp_error("[PacketFilter::PacketFilter(string)] Error, packet filter in the xml has a bad structure");

        }
        catch(const out_of_range& oor) {
            opp_error("[PacketFilter::PacketFilter(string)] Out of range exception raised during the building of an elementary or a compound block");
        }
    }
    
    // simple consistency check
    if (filterBlocks.size() != filterOperators.size()+1) {
        opp_error("[PacketFilter::PacketFilter(string)] Error, the number of packet-filter's block is inconsistent with that of the logical operators");
    }
    
    // set packet-filter's minimum layer
    setPacketFilterLayer();
    
}


void PacketFilter::setPacketFilterLayer()
{
    // find the minimum layer among the blocks
    packetFilterMinimumLayer = 5;
    
    block_t blockType;
    int blockLayer;
    for (size_t i = 0; i < filterBlocks.size(); i++) {
        
        blockType = filterBlocks[i]->getType();
        // solve the block
        switch (blockType) {
            case block_t::ELEMENTARY: {
                blockLayer = ((ElementaryBlock*)filterBlocks[i])->getLayer();
                break;   
            }
            case block_t::COMPOUND: {
                blockLayer = ((CompoundBlock*)filterBlocks[i])->getLayer();
                break;
            }
            default: {
                opp_error("[PacketFilter::setPacketFilterLayer()] Can't recognize the type of the filter block");
            }
        }
        
        if (blockLayer < packetFilterMinimumLayer) {
            packetFilterMinimumLayer = blockLayer;
        }
        
    }
    
}


PacketFilter::~PacketFilter()
{    
    // delete blocks composing the packet-filter
    for (size_t i = 0; i < filterBlocks.size(); i++) {
        delete filterBlocks[i];
    }
}


bool PacketFilter::passPreMatchChecks(cMessage* msg) const
{
    cPacket* packet = (cPacket*)msg;
    
	// <A.S>
	// check if the condition is based on controlInfo (independent of the layer)
	if(packetFilterMinimumLayer == 0) {
        return true;
	}
	
    // check if the packet was already filtered before
	if (packet->par("isFiltered").boolValue()) {		
		return false;	
	}

	// postpone the attack after encapsulation
	if (getPacketLayer(packet) > packetFilterMinimumLayer) {
		return false;
	}
    
    return true;
}


bool PacketFilter::matchPacketFilter(cMessage* msg) const
{    
    // check if the packet can be filtered
    if (passPreMatchChecks(msg) == false) {
        return false;
    }

    // solve all blocks for the current message
    vector<bool> solved;
    for (size_t i = 0; i < filterBlocks.size(); i++) {
        // get the type of the block
        block_t blockType = filterBlocks[i]->getType();
        // solve the block
        switch (blockType) {
            case block_t::ELEMENTARY: {
                ElementaryBlock* block = (ElementaryBlock*)filterBlocks[i];
                solved.push_back(block->solveFilterBlock(msg));
                break;   
            }
            case block_t::COMPOUND: {
                CompoundBlock* block = (CompoundBlock*)filterBlocks[i];
                solved.push_back(block->solveFilterBlock(msg));
                break;
            }
        }
    }

    // solve the packet-filter
    bool result = solved[0];
    for (size_t i = 0; i < filterOperators.size(); i++) {
        
        if (filterOperators[i] == "OR") {
            if (result == true) {
                return true;
            }
            else {
                result = solved[i+1];
            }
        }
        else {
            result = result && solved[i+1];
        }
        
    }
    
    return result;
}

