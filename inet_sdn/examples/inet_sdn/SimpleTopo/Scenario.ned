package inet.examples.inet_sdn.SimpleTopo;

import inet.nodes.inet.StandardHost;
import inet.networklayer.autorouting.ipv4.FlatNetworkConfigurator;
import inet.util.ThruputMeteringChannel;
import inet.globalfilter.GlobalFilter;
import inet.openflow.nodes.*;

network Scenario
{
	parameters:
		string attackConfigurationFile = default("none");
	@display("bgb=600,450");
	types:
        channel ethernetline extends ThruputMeteringChannel
        {
            delay = 1us;
            datarate = 100Mbps;
            thruputDisplayFormat = "u";
        }
	submodules:
		configurator: FlatNetworkConfigurator {
            @display("p=50,400");
        }
        globalFilter: GlobalFilter {
			@display("p=500,41;");
		}
        client: StandardHost {
            @display("p=30,300;i=device/laptop");
        }
        server: StandardHost {
            @display("p=570,300;i=device/server");
        }       
                
        switch: Open_Flow_Switch_SEA  {
            @display("p=350,300");
        }
        controller: Open_Flow_Controller_SEA {
            @display("p=350,41");
        }


        
	connections allowunconnected:
		switch.ethg++  <--> ethernetline <--> client.ethg++;
		switch.ethg++  <--> ethernetline <--> server.ethg++;

		controller.ethg++ <--> ethernetline <--> switch.gate_controller++;
		
		globalFilter.nodes++ <--> client.global_filter;
		globalFilter.nodes++ <--> server.global_filter;
				
		globalFilter.nodes++ <--> controller.global_filter;
		globalFilter.nodes++ <--> switch.global_filter;
        		
}
