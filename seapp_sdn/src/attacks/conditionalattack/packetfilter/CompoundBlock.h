/**
 * @file CompoundBlock.h
 * 
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 *
 * @brief The class CompoundBlock models compound-blocks composing 
 *        the packet filter.
 * 
 * @details A compound-block has the following template:
 *          [ ( fullPath arithOperator operand ) logicOperator operand ]
 *          ^ ^     ^         ^            ^   ^      ^           ^    ^
 *          0 1     2         3            4   5      6           7    8
 */
 

#ifndef COMPOUNDBLOCK_H
#define COMPOUNDBLOCK_H


#include "FilterBlock.h"
#include "omnetpp.h"


using namespace std;


class CompoundBlock : public FilterBlock {
    public:
        // size of a compound block
        static const size_t compoundBlockSize = 9;
     
    public:
        /**
         * @brief Constructor.
         * @param blockElements Is the vector of elements composing the 
         *        compound block.
         */
        CompoundBlock(vector<string> blockElements);
        
        /**
         * @brief Destructor.
         */
        ~CompoundBlock();
        
        /**
         * @brief Gets the layer from the block.
         * @return Returns the layer of the block.
         */
        int getLayer() const;
        
        /**
         * @brief Solves the compound block by using the infos 
         *        contained in the current message.
         * @param msg Is the pointer-to the message to test.
         * @return Returns true if the message matches the compound 
         *         block, false otherwise.
         */
        bool solveFilterBlock(cMessage* msg) const;
};

#endif
