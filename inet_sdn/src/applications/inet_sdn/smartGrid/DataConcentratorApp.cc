#include "DataConcentratorApp.h"
#include "MeasurementData_m.h"
#include "IPvXAddressResolver.h"


Define_Module(DataConcentratorApp);

simsignal_t DataConcentratorApp::rcvdPkSignal = registerSignal("rcvdPk");
simsignal_t DataConcentratorApp::sentPkSignal = registerSignal("sentPk");

DataConcentratorApp::DataConcentratorApp() {
    report = new Report();
    reportMsg = NULL;
}

DataConcentratorApp::~DataConcentratorApp() {
    delete report;
}

void DataConcentratorApp::initialize(int stage) {
    cSimpleModule::initialize(stage);
    if (stage == 0) {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);
    }
    
    bind();

    getParentModule()->subscribe("MeasurementData", this);
    
    interval = par("intervalReport"); 
    reportMsg = new cMessage("SendReport");
    reportMsg->setKind(SEND);
    scheduleAt(simTime()+interval, reportMsg);
    

    startTime = par("startTime");
    timeoutMsg = new cMessage("timer");
    timeoutMsg->setKind(CONNECT);
    scheduleAt(simTime()+startTime, timeoutMsg);
}

void DataConcentratorApp::bind() {
    const char *localAddress = par("localAddress");
    int localPort = par("localPort");
        
    socket.setOutputGate(gate("tcpOut"));
    socket.setDataTransferMode(TCP_TRANSFER_OBJECT);
    socket.bind(*localAddress ? IPvXAddressResolver().resolve(localAddress) : IPvXAddress(), localPort);
}

void DataConcentratorApp::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        switch(msg->getKind()) {
            case CONNECT: 
                connect(); 
                break;
            case SEND: 
                sendReportToRTU(); 
                break;
            default: throw cRuntimeError("Invalid kind %d in self message", (int)msg->getKind());
        }
    }    
    displayGUI();
    delete msg;

}
void DataConcentratorApp::sendReportToRTU() {
    double avg = 0;
    if (report->getCounter() != 0)     
        avg = report->getAvg();
                
        report->resetData();
            
        MeasurementData *data = new MeasurementData("MeasurementData");
        data->setEnergyConsumption(avg);
        data->setByteLength(1);
        data->setTimestamp(simTime());
        data->setKind(TCP_C_SEND);
            
        numSent++;
        emit(sentPkSignal, data);
            
        socket.send(data);
        
        cMessage *report = new cMessage("SendReport");
        report->setKind(SEND);
        scheduleAt(simTime()+interval, report);
}


void DataConcentratorApp::receiveSignal(cComponent *src, simsignal_t id, cObject *obj) { 
    Enter_Method_Silent();
    // Notification from Smart Meters
	string signalName(getSignalName(id));
	if (signalName == "MeasurementData") { 
        if (dynamic_cast<MeasurementData *>(obj) != NULL) {
		   MeasurementData *data = (MeasurementData *) obj;
		   report->updateRecord(data->getEnergyConsumption());           
		   delete data;
		}
	}
}

void DataConcentratorApp::connect() {
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

void DataConcentratorApp::displayGUI() {
    if (ev.isGUI()) {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void DataConcentratorApp::finish() { }


