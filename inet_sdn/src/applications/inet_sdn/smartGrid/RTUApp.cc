#include "RTUApp.h"
#include "MeasurementData_m.h"
#include "IPvXAddressResolver.h"

Define_Module(RTUApp);

simsignal_t RTUApp::rcvdPkSignal = registerSignal("rcvdPk");
simsignal_t RTUApp::sentPkSignal = registerSignal("sentPk");

RTUApp::RTUApp() {
    report = new Report();
}

RTUApp::~RTUApp() {
    delete report;
}

void RTUApp::initialize(int stage) {
    cSimpleModule::initialize(stage);
    if (stage == 0) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);
    }
    startListening();
   
    interval = par("intervalReport");
    reportMsg = new cMessage("SendReport");
    reportMsg->setKind(SEND);
    scheduleAt(simTime()+interval, reportMsg);
    
    startTime = par("startTime");
    timeoutMsg = new cMessage("timer");
    timeoutMsg->setKind(CONNECT);
    scheduleAt(simTime()+startTime, timeoutMsg);
    
}

void RTUApp::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        switch (msg->getKind()) {
            case CONNECT: 
                connect();
                break;
            case SEND:
                sendReportToDSO();
                break;
            default: throw cRuntimeError("Invalid kind %d in self message", (int)msg->getKind());
        }
    } else {
        if (dynamic_cast<MeasurementData *>(msg) != NULL) {
            MeasurementData *data = (MeasurementData *) msg;
            emit(rcvdPkSignal, msg);
            numReceived++;
            report->updateRecord(data->getEnergyConsumption());
        }
    }
   
    displayGUI();
    delete msg;
}

void RTUApp::sendReportToDSO() {
    MeasurementData *data = new MeasurementData("MeasurementData");
    data->setEnergyConsumption(report->getAvg());
    report->resetData();
    data->setByteLength(1);
    data->setTimestamp(simTime());
    data->setKind(TCP_C_SEND);
                
    emit(sentPkSignal, data);
    numSent++;
        
    socket.send(data); 
    
    cMessage *report = new cMessage("SendReport");
    report->setKind(SEND);
    scheduleAt(simTime()+interval, report);
}

void RTUApp::startListening() {
    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
    
    socket.setOutputGate(gate("tcpOut"));
    socket.setDataTransferMode(TCP_TRANSFER_OBJECT);
    socket.bind(localAddress[0] ? IPvXAddress(localAddress) : IPvXAddress(), localPort);
    socket.listen();
}

void RTUApp::connect() {
    socket.renewSocket();

    const char *connectAddress = par("connectAddress");
    int connectPort = par("connectPort");

    IPvXAddress destination;
    IPvXAddressResolver().tryResolve(connectAddress, destination);
    if (destination.isUnspecified())
        EV << "cannot resolve destination address: " << connectAddress << endl;
    else 
        socket.connect(destination, connectPort);
}

void RTUApp::displayGUI() {
    if (ev.isGUI()) {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void RTUApp::finish() { }
