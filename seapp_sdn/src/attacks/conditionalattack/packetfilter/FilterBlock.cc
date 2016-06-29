/**
 * @file	FilterBlock.cc
 * @author	Francesco Racciatti <racciatti.francesco@gmail.com>
 */


#include "FilterBlock.h"


FilterBlock::FilterBlock(block_t blockType, vector<string> blockElements)
{
    this->blockType = blockType;
    this->blockElements = blockElements;
}


FilterBlock::~FilterBlock()
{
    
}


block_t FilterBlock::getType() const
{
    return blockType;
}
