/**
 * @file PacketFilter.h
 * @author Francesco Racciatti <racciatti.francesco@gmail.com>
 * @brief PacketFilter class models the packet-filter.
 */
 

#ifndef PACKETFILTER_H
#define PACKETFILTER_H


#include "FilterBlock.h"


using namespace std;


class PacketFilter {
    private:
        // pointers-to blocks building the packet-filter
        vector<FilterBlock*> filterBlocks;
        // operators between blocks building the packet-filter
        vector<string> filterOperators;
        // minimum layer of the packet-filter
        int packetFilterMinimumLayer;
        
    private:
        /**
         * @brief Sets the packet-filter minimum layer.
         */
        void setPacketFilterLayer();

        /**
         * @brief Checks if the message can be safely filtered.
         * @param msg Is the pointer-to the message to filter.
         */
        bool passPreMatchChecks(cMessage* msg) const;

    public:
        /**
         * @brief Constructor.
         * @param monolithicPacketFilter Is the monolithic 
         *        packet-filter.
         */
        PacketFilter(string monolithicPacketFilter);
        
        /**
         * @brief Destructor.
         */
        ~PacketFilter();
        
        /**
         * @brief Checks if the current message matches the 
         *        packet-filter.
         * @param msg Is the pointer-to the current message.
         * @return Returns true if the current message matches the 
         *         packet-filter, false otherwise.
         */
        bool matchPacketFilter(cMessage* msg) const;
};

#endif
