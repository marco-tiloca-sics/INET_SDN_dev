package inet.nodes.inet_sdn.smartGrid;

import inet.nodes.inet.StandardHost;
import inet.applications.inet_sdn.smartGrid.SmartMeter;
import inet.applications.inet_sdn.smartGrid.DataConcentratorApp;
import inet.transport.ITCP;

module DataConcentrator extends StandardHost {
    parameters:
        @display("i=device/pc2");
        int numSmartMeters = default (2);
        
    submodules:
        smartMeter[numSmartMeters]: SmartMeter {
            @display("p=400,100,r,80");
        }
        data_concentrator: DataConcentratorApp;
        tcp: <tcpType> like ITCP {}
        
    connections allowunconnected:
        tcp.appOut++ --> localFilter.app_tcp_inf$i++;
		tcp.appIn++ <-- localFilter.app_tcp_inf$o++;
		data_concentrator.tcpIn <-- localFilter.app_tcp_sup$o++;
		data_concentrator.tcpOut --> localFilter.app_tcp_sup$i++;
		
		tcp.ipOut --> localFilter.tcp_net_sup$i++;
		tcp.ipIn <-- localFilter.tcp_net_sup$o++;
		networkLayer.transportOut++ --> localFilter.tcp_net_inf$i++;
		networkLayer.transportIn++ <-- localFilter.tcp_net_inf$o++;
}

