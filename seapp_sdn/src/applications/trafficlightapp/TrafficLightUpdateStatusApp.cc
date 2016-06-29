// author: Francesco Racciatti <racciatti.francesco@gmail.com>


#include "TrafficLightUpdateStatusApp.h"
#include <math.h>
#include <fstream>
#include <iostream>


Define_Module(TrafficLightUpdateStatusApp);


void TrafficLightUpdateStatusApp::initialize() {
 		
 		
 		cMessage* msg = new cMessage("colorize", 0);
 		scheduleAt(0, msg);

}


void TrafficLightUpdateStatusApp::handleMessage(cMessage *msg) {

	string roadType = getParentModule()->par("roadType").stringValue();

	double t = (simTime()).dbl();
	double scheduleTime = t + updatePeriod;
	
	double periodGreen =  getParentModule()->par("periodGreen").doubleValue();
	double periodYellow = getParentModule()->par("periodYellow").doubleValue();
	double periodRed = getParentModule()->par("periodRed").doubleValue();
	double sequencePeriod = periodGreen + periodYellow + periodRed; 
	
	double timeInActualSequence = fmod(t, sequencePeriod);
	
	// maind road sequence:	g->y->r
	if (roadType == "m") {
		if (timeInActualSequence < periodGreen) {
			setGreen();
		}
		else {
			if (timeInActualSequence < (periodGreen + periodYellow)) {
				setYellow();
			}
			else {
				setRed();
			}
		}
	}
	// secondary road sequence: r->g->y
	else {
		if (timeInActualSequence < periodRed) {
			setRed();
		}
		else {
			if (timeInActualSequence < (periodRed + periodGreen)) {
				setGreen();
			}
			else {
				setYellow();
			}
		}
	}
	
	string color = getParentModule()->par("status").stringValue();
	updateFile(t, color);
		
	cMessage* nextMsg = new cMessage ("updateStatus", 0);
	scheduleAt(scheduleTime, nextMsg);

	delete msg;
	
}

void TrafficLightUpdateStatusApp::finish() {
 

   
}


void TrafficLightUpdateStatusApp::setRed() {

	getParentModule()->par("status").setStringValue("r");
	getParentModule()->getDisplayString().setTagArg("i",0,"misc/tl-r");

}


void TrafficLightUpdateStatusApp::setYellow() {

	getParentModule()->par("status").setStringValue("y");
	getParentModule()->getDisplayString().setTagArg("i",0,"misc/tl-y");

}


void TrafficLightUpdateStatusApp::setGreen() {

	getParentModule()->par("status").setStringValue("g");
	getParentModule()->getDisplayString().setTagArg("i",0,"misc/tl-g");

}


void TrafficLightUpdateStatusApp::updateFile(double t, string color) {

	ofstream outputFile;
	string fileName;
	string number;
	number.assign(getParentModule()->par("number").stringValue());

	int convertedColor;
	
	if (color == "g") {
		convertedColor = 5;
	}
	if (color == "y") {
		convertedColor = 0;
	}
	if (color == "r") {
		convertedColor = -5;
	}


	if (number == "-1") {
		opp_error("Traffic Light hasn't a proper number, please assign a number to each traffic light");
	}
	else {

		fileName.assign("tl");
		fileName.append(number);
		
		outputFile.open(fileName.c_str(), ios::app);
		outputFile << t << " " << convertedColor << '\n';
		outputFile.close();
	
	}

}






