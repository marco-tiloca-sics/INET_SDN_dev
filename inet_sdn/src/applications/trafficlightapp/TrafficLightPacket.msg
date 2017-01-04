// author: Francesco Racciatti <racciatti.francesco@gmail.com>


cplusplus {{
#include "INETDefs.h"
}}


packet TrafficLightBase {
	
};

packet TrafficLightStatus extends TrafficLightBase {
	double time;
	string roadType;
	double periodGreen;
	double periodYellow;
	double periodRed;		
};

packet TrafficLightCmd extends TrafficLightBase {
	long sequenceNumber;
	double periodGreen;
	double periodYellow;
	double periodRed;
	
};
