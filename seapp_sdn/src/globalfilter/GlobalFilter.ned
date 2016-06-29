//	
// author: Francesco Racciatti
// email: racciatti.francesco@gmail.com
//


package inet.globalfilter;

simple GlobalFilter {

 parameters:
	bool collectTraceInfo = default (false);
	bool evaluateAttacks = default (false);

    //string applicationName = default("");
    //string routingProtocolName = default("");
    //string macProtocolName = default("");

	string applicationName = default("");
	string layer4ProtocolName = default("");
	string layer3ProtocolName = default("");
    string layer2ProtocolName = default("");
    
  gates:
	inout nodes[];
	output toLogger;

}
 
