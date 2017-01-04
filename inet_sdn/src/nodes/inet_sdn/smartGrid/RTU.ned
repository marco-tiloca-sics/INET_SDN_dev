package inet.nodes.inet_sdn.smartGrid;

import inet.nodes.inet.StandardHost;
import inet.applications.inet_sdn.smartGrid.RTUApp;
import inet.transport.ITCP;

module RTU extends StandardHost {
    parameters:
        @display("i=device/pc2");
    submodules:
        rtu: RTUApp;
        tcp: <tcpType> like ITCP {}
    connections allowunconnected:    
        tcp.appOut++ --> localFilter.app_tcp_inf$i++;
		tcp.appIn++ <-- localFilter.app_tcp_inf$o++;
		rtu.tcpIn <-- localFilter.app_tcp_sup$o++;
		rtu.tcpOut --> localFilter.app_tcp_sup$i++;
		
        tcp.ipOut --> localFilter.tcp_net_sup$i++;
		tcp.ipIn <-- localFilter.tcp_net_sup$o++;
		networkLayer.transportOut++ --> localFilter.tcp_net_inf$i++;
		networkLayer.transportIn++ <-- localFilter.tcp_net_inf$o++;
}

