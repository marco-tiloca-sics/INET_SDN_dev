//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package inet.ofomnet.openflow.nodes;

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;
import inet.transport.ITCP;
import inet.ofomnet.openflow.ctrlApps.CtrlBehavior;
import inet.ofomnet.openflow.ctrlApps.*;
import inet.ofomnet.openflow.*;
import inet.ofomnet.openflow.attackDetection.*;
import inet.ofomnet.openflow.attackDetection.methods.Entropy;
import inet.ofomnet.openflow.mitigation.*;

//
// TODO documentation
//
module Open_Flow_Controller extends inet.nodes.inet.StandardHost
{
    parameters:
        @node();
        @labels(node,ethernet-node);
        @display("i=device/server;bgb=843,453;i2=OpenFlow-Logo-small");
        //int numTcpApps = default(0);
        //int numUdpApps = default(0);
        //int numSctpApps = default(0);
        string tcpAppType = default("n/a");
        string udpAppType = default("n/a");
        string sctpAppType = default("n/a");
        string behavior = default("Switch");
        //string tcpType = default("TCP");  // tcp implementation (e.g. TCP, TCP_old, TCP_NSC)
        //bool IPForward = default(false);
        int namid = default(-1);
        //string routingFile = default("");
        //int numExtInterfaces = default(0);
        double serviceTime @unit("s") = default(0s);
        double statsInterval @unit("s") = default(5s);
        int domainID = default(-1);

    gates:
        //inout ethg[] @labels(EtherFrame-conn);
    submodules:
		//notificationBoard: NotificationBoard {
        //    parameters:
        //        @display("p=57,54");
        //}
        //interfaceTable: InterfaceTable {
        //    parameters:
        //        @display("p=57,134");
        //}
        //routingTable: RoutingTable {
        //    parameters:
        //        IPForward = IPForward;
        //        routerId = "";
        //        routingFile = routingFile;
        //        @display("p=57,214");
        //}
        //networkLayer: NetworkLayer {
         //   parameters:
         //       proxyARP = false;
        //        @display("p=336,230;q=queue");
        //    gates:
        //        ifIn[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        //        ifOut[sizeof(pppg)+sizeof(ethg)+numExtInterfaces];
        //}
        //eth[sizeof(ethg)]: EthernetInterface {
        //    parameters:
         //       @display("p=113,376,row,90;q=txQueue");
        //}
        
		tcp: <tcpType> like ITCP {
            parameters:
                @display("p=200,154");
        }
        ofa_controller: OFA_controller {
            @display("p=200,54");
            serviceTime = serviceTime;
            statsInterval = statsInterval;
        }
        ctrlApp: <behavior> like CtrlBehavior {
            @display("p=300,54");
        }
        attackDetection: AttackDetection {
        	@display("p=400,54");
        }
        entropy: Entropy {
            @display("p=500,54");
        }
        mitigation: Mitigation{
            @display("p=600,54");
        }
        
    connections allowunconnected:    
    	
        //tcp.appOut++ --> ofa_controller.tcpIn;
        //ofa_controller.tcpOut --> tcp.appIn++;
        //------local filter ---
        tcp.appOut++ --> localFilter.app_tcp_inf$i++;
		tcp.appIn++ <-- localFilter.app_tcp_inf$o++;
		ofa_controller.tcpIn <-- localFilter.app_tcp_sup$o++;
		ofa_controller.tcpOut --> localFilter.app_tcp_sup$i++;
		
        //tcp.ipOut --> networkLayer.transportIn++;
        //tcp.ipIn <-- networkLayer.transportOut++;
        //--------local filter-----
        tcp.ipOut --> localFilter.tcp_net_sup$i++;
		tcp.ipIn <-- localFilter.tcp_net_sup$o++;
		networkLayer.transportOut++ --> localFilter.tcp_net_inf$i++;
		networkLayer.transportIn++ <-- localFilter.tcp_net_inf$o++;
        
        
        //for i=0..sizeof(ethg)-1 {
        //    ethg[i] <--> eth[i].phys;
        //    eth[i].upperLayerOut --> networkLayer.ifIn[i];
        //    eth[i].upperLayerIn <-- networkLayer.ifOut[i];
        //}
        
}


