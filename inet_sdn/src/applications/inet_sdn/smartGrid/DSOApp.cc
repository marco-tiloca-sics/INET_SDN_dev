#include "DSOApp.h"
#include "MeasurementData_m.h"
#include "IPvXAddressResolver.h"

Define_Module(DSOApp);

simsignal_t DSOApp::rcvdPkSignal = registerSignal("rcvdPk");

DSOApp::DSOApp() {
    report = new Report();
}

DSOApp::~DSOApp() {
    delete report;
}

void DSOApp::initialize(int stage) {
    cSimpleModule::initialize(stage);
    if (stage == 0) {
        numReceived = 0;
        WATCH(numReceived);
    }
    startListening();
}

void DSOApp::startListening() {
    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    socket.setOutputGate(gate("tcpOut"));
    socket.setDataTransferMode(TCP_TRANSFER_OBJECT);
    socket.bind(localAddress[0] ? IPvXAddress(localAddress) : IPvXAddress(), localPort);
    socket.listen();
}  

void DSOApp::handleMessage(cMessage *msg) {
    if (dynamic_cast<MeasurementData *>(msg) != NULL) {
        MeasurementData *data = (MeasurementData *) msg;
        report->updateRecord(data->getEnergyConsumption());
        
        std::cout<< simTime() << ": pira reports\n";
        numReceived++;
        emit(rcvdPkSignal, data);
    }
    
    displayGUI();
        
    delete msg;
}


void DSOApp::displayGUI() {
    if (ev.isGUI()) {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\n", numReceived);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void DSOApp::finish() { }

