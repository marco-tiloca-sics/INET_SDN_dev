package inet.examples.inet.broadcast;

import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.ethernet.EtherSwitch;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;
import inet.globalfilter.GlobalFilter;


network UDPBroadcastNetwork
{
    parameters:
        int numTargets;
        string attackConfigurationFile = default("none");
    types:
        channel C extends DatarateChannel
        {
            datarate = 10Mbps;
            delay = 0.1us;
        }
    submodules:
    	globalFilter: GlobalFilter {
			parameters:
				@display("p=30,50");
    	}
    
        target[numTargets]: StandardHost;
        
        client: StandardHost {
            parameters:
                @display("p=62,115;i=device/pc2");
        }
        R1: Router {
            parameters:
                @display("p=180,115");
        }
        R2: Router {
			parameters:
            @display("p=279,115");
        }
        
        configurator: IPv4NetworkConfigurator {
            parameters:
                config = xmldoc("config.xml");
                @display("p=62,31");
        }
       
       
       	switch: EtherSwitch {
       		@display("p=350,115");
       	}	
       	
        nonTarget: StandardHost {
            @display("p=389,31");
        }

    connections allowunconnected:
		globalFilter.nodes++ <--> client.global_filter;
       	globalFilter.nodes++ <--> R1.global_filter;
        globalFilter.nodes++ <--> R2.global_filter;
        globalFilter.nodes++ <--> nonTarget.global_filter;

        client.pppg++ <--> C <--> R1.pppg++;
        R1.pppg++ <--> C <--> R2.pppg++;
        R2.pppg++ <--> C <--> nonTarget.pppg++;
        R2.ethg++ <--> C <--> switch.ethg++;

        for i=0..numTargets-1 {
            switch.ethg++ <--> C <--> target[i].ethg++;
        }
}
