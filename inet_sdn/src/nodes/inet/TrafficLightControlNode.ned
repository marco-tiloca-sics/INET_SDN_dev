//
// Copyright (C) 2004 Andras Varga
// Copyright (C) 2000 Institut fuer Telematik, Universitaet Karlsruhe
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

package inet.nodes.inet;

import inet.applications.IPingApp;
import inet.applications.ISCTPApp;
import inet.applications.ITCPApp;
import inet.applications.IUDPApp;
import inet.transport.ISCTP;
import inet.transport.ITCP;
import inet.transport.IUDP;

import inet.applications.trafficlightapp.TrafficLightSinkApp;
import inet.applications.trafficlightapp.TrafficLightBasicApp;
import inet.applications.trafficlightapp.CommandNodeSinkApp;
import inet.applications.trafficlightapp.CommandNodeBasicApp;

module TrafficLightControlNode extends NodeBase
{
    parameters:
        @display("i=device/pc2");
        int numTcpApps = default(0);  // no of TCP apps. Specify the app types in INI file with tcpApp[0..1].typename="TCPEchoApp" syntax
        int numUdpApps = default(1);  // no of UDP apps. Specify the app types in INI file with udpApp[0..1].typename="UDPVideoStreamCli" syntax
        int numSctpApps = default(0); // no of SCTP apps. Specify the app types in INI file with sctpApp[0..1].typename="SCTPServer" syntax
        int numPingApps = default(0);  // no of PING apps. Specify the app types in INI file with pingApp[0..1].typename="PingApp" syntax
        bool hasTcp = default(numTcpApps>0);
        bool hasUdp = default(numUdpApps>0);
        bool hasSctp = default(numSctpApps>0);
        string tcpType = default(firstAvailable("TCP", "TCP_lwIP", "TCP_NSC", "TCP_None"));  // tcp implementation (e.g. ~TCP, ~TCP_lwIP, ~TCP_NSC) or ~TCPSpoof
        string udpType = default(firstAvailable("UDP","UDP_None"));
        string sctpType = default(firstAvailable("SCTP","SCTP_None"));
        IPForward = default(false);  // disable routing by default
        networkLayer.proxyARP = default(false);
    submodules:
     
     	controlNodeSink1: CommandNodeSinkApp;
     
     	controlNodeSink2: CommandNodeSinkApp;
     	
     	controlNodeSink3: CommandNodeSinkApp;
   
   		sender1: CommandNodeBasicApp;
   		sender2: CommandNodeBasicApp;
   		sender3: CommandNodeBasicApp;
   
     
   
     	//controlNodeCommandApp: TrafficLightBasicApp;
     
        udp: <udpType> like IUDP{
            parameters:
                @display("p=329,141");
        }
    
    connections allowunconnected:
	
	
		//controlNodeCommandApp.udpOut --> localFilter.app_udp_sup$i++;
		//controlNodeCommandApp.udpIn <-- localFilter.app_udp_sup$o++;
		
		controlNodeSink1.udpOut --> localFilter.app_udp_sup$i++;
		controlNodeSink1.udpIn <-- localFilter.app_udp_sup$o++;
		controlNodeSink2.udpOut --> localFilter.app_udp_sup$i++;
		controlNodeSink2.udpIn <-- localFilter.app_udp_sup$o++;
		controlNodeSink3.udpOut --> localFilter.app_udp_sup$i++;
		controlNodeSink3.udpIn <-- localFilter.app_udp_sup$o++;

		sender1.udpOut --> localFilter.app_udp_sup$i++;
		sender1.udpIn <-- localFilter.app_udp_sup$o++;
		sender2.udpOut --> localFilter.app_udp_sup$i++;
		sender2.udpIn <-- localFilter.app_udp_sup$o++;
		sender3.udpOut --> localFilter.app_udp_sup$i++;
		sender3.udpIn <-- localFilter.app_udp_sup$o++;

			
		//udp.appOut++ --> localFilter.app_udp_inf$i++;
		//udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		udp.appOut++ --> localFilter.app_udp_inf$i++;
		udp.appIn++ <-- localFilter.app_udp_inf$o++;
		
		// <F.R.> 
		udp.ipOut --> localFilter.udp_net_sup$i++;
		udp.ipIn <-- localFilter.udp_net_sup$o++;
		networkLayer.transportOut++ --> localFilter.udp_net_inf$i++;
		networkLayer.transportIn++ <-- localFilter.udp_net_inf$o++;

        
        
        
        
        
}
