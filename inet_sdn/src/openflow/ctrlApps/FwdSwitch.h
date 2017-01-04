/*
 * FwdSwitch.h
 *     
 */

#ifndef FWDSWITCH_H_
#define FWDSWITCH_H_

#include "OFA_controller.h"
#include "MACAddress.h"

struct MAC_connID
{
    MACAddress address;
    int connID;
};

struct ltmatch
{
    bool operator() (const MAC_connID m1, const MAC_connID m2) const
    {
        return m1.address.compareTo(m2.address);
    }
};

class FwdSwitch: public cSimpleModule, public cListener
{
public:
    FwdSwitch();
    ~FwdSwitch();
    void receiveSignal(cComponent *src, simsignal_t id, cObject *obj);

protected:
    virtual void initialize();


private:
    OFA_controller *controller;
    std::multimap<MAC_connID, uint32_t, ltmatch> mac_to_port;
};


#endif /* FWDSWITCH_H_ */
