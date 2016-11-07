package inet.examples.inet_sdn.scenario;

import inet.nodes.inet.StandardHost;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.util.ThruputMeteringChannel;
import inet.globalfilter.GlobalFilter;
import inet.ofomnet.openflow.nodes.*;




network Topo
{
	parameters:		
		string attackConfigurationFile = default("none");
		
	@display("bgb=2000,1000");
	types:
        channel ethernetline extends ThruputMeteringChannel
        {
            delay = 1us;
            datarate = 100Mbps;
            //thruputDisplayFormat = "u";
        }
	submodules:
		globalFilter: GlobalFilter {
			@display("p=1800,200");
		}

		configurator: FlatNetworkConfigurator {
            parameters:
                @display("p=1800, 500");
        }
        client: StandardHost {
           @display("p=150,250;i=device/laptop");
        }

        server: StandardHost {
            @display("p=1500,350;i=device/server");
        }
        
        s1: Open_Flow_Switch  {
            @display("p=1000,650");
        }
        controller: Open_Flow_Controller {
            @display("p=1000,100");
        }

        
	connections allowunconnected:
        
        client.ethg++ <--> ethernetline <--> s1.ethg++;
        server.ethg++ <--> ethernetline <--> s1.ethg++;
     

		controller.ethg++ <--> ethernetline <--> s1.gate_controller++;
		
		globalFilter.nodes++ <--> client.global_filter;
		globalFilter.nodes++ <--> server.global_filter;
		
		
}
