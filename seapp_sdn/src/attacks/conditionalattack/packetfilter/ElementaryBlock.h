/**
 * @file ElementaryBlock.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief ElementaryBlock models elementary blocks composing the 
 *        packet filter.
 * @details An elementary block has the following template:
 *          [ : layerName.fieldName.subFieldName... : comparisonOperator : operand : ]
 */
 

#ifndef ELEMENTARYBLOCK_H
#define ELEMENTARYBLOCK_H


#include "FilterBlock.h"
#include "omnetpp.h"


using namespace std;


class ElementaryBlock : public FilterBlock {
    public:
        // size of an elementary block
        static const size_t elementaryBlockSize = 5;
     
    public:
        /**
         * @brief Constructor.
         * @param blockElements Is the vector that contains the 
         *        elements forming the elementary block.
         */
        ElementaryBlock(vector<string> blockElements);
        
        /**
         * @brief Destructor.
         */
        ~ElementaryBlock();
        
        /**
         * @brief Gets the layer from the block.
         * @return Returns the layer of the block.
         */
        int getLayer() const;
        
        /**
         * @brief Solves the elementary block by using the infos 
         *        contained in the current message.
         * @param msg Is the pointer-to the message to test.
         * @return Returns true if the message matches the elementary 
         *         block, false otherwise.
         */
        bool solveFilterBlock(cMessage* msg) const;
};

#endif
