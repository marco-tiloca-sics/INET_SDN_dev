#include "SmartMeter.h"
#include "MeasurementData_m.h"
#include "seapputils.h"

Define_Module(SmartMeter);

void SmartMeter::initialize() {
	measurementDataSignal = registerSignal("MeasurementData");
	nextReading();
}

void SmartMeter::finish() {
}

void SmartMeter::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {      
        MeasurementData *data = new MeasurementData();
        data->setByteLength(1);
        data->setEnergyConsumption(0.40);
        data->setTimestamp(simTime());
        emit(measurementDataSignal, data);     
        
        delete msg;
        nextReading();
    }
}

void SmartMeter::nextReading() {
    cMessage *reading = new cMessage("ReadingData");
    int randomInterval = generateRandomIntValue(1,3);
    scheduleAt(simTime() + randomInterval, reading);
}
