/**
 * @file	FilterBlock.h
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief	FilterBlock models blocks composing the packet filter.
 */
 

#ifndef FILTERBLOCK_H
#define FILTERBLOCK_H

#include <vector>
#include <string>
#include "omnetpp.h"


using namespace std;


enum class block_t {
    ELEMENTARY = 0,
    COMPOUND,
};


class FilterBlock {
    protected:
        // type of block
        block_t blockType;
        // elements forming the filter's block
        vector<string> blockElements;
     
    protected:
        /**
         * @brief Constructor.
         * @param blockElements Is the vector of elements forming the 
         *        filter's block.
         */
        FilterBlock(block_t blockType, vector<string> blockElements);
        
    public:
        /**
         * @brief Destructor.
         */
        virtual ~FilterBlock();
        
        /**
         * @brief Getter method, gets the type of the block.
         * @return Returns the type of the block.
         */
        block_t getType() const;
        
        /**
         * @brief Pure virtual method, gets the layer from the block.
         * @return Returns the layer of the block.
         */
        virtual int getLayer() const = 0;
        
        /**
         * @brief Pure virtual method, solves the block by using the 
         *        infos contained in the current packet's fields.
         * @param packet Pointer-to the packet to test.
         * @return Returns true if the current packet matches the 
         *         block, false otherwise.
         */
        virtual bool solveFilterBlock(cMessage* msg) const = 0;
};

#endif
