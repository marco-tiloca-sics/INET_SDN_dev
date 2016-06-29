// author: Francesco Racciatti <racciatti.francesco@gmail.com>


package inet.examples.inet.intersection;

import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import ned.DatarateChannel;
import inet.globalfilter.GlobalFilter;
import inet.nodes.inet.TrafficLight;
import inet.nodes.inet.TrafficLightControlNode;

network intersection
{
	parameters:
		string attackConfigurationFile = default("none");

	types:
		channel ethernet extends DatarateChannel {
			delay = normal(0.00015s, 0.00005s);
			datarate = 10Mbps;
		}
        
		channel fiberrouterline extends DatarateChannel {
			delay = normal(0.000001s, 0.0000001s);
			datarate = 512Mbps;
		}
		
	submodules:

		configurator: IPv4NetworkConfigurator;

		globalFilter: GlobalFilter{
			parameters:
				@display("p=75,275");
		};
	   		
		tl1: TrafficLight {
			parameters:
				number="1";
				roadType ="m";
				@display("p=50,50");
				@display("i=misc/tl");
		};
		
		tl2: TrafficLight {
			parameters:
				number="2";
				roadType ="m";
				@display("p=50,150");
				@display("i=misc/tl");
		};
		
		tl3: TrafficLight {
			parameters:
				number="3";
				roadType ="s";
				@display("p=100,100");
				@display("i=misc/tl");
		};
		
        
		r1: Router {
			parameters:
				@display("i=abstract/router");
				@display("p=300,75");
		};

		cn : TrafficLightControlNode{
			parameters:
				@display("i=device/pc");
				@display("p=250,220");
		};
		
	connections allowunconnected:
		// traffic lights connections
		tl1.pppg++ <--> ethernet <--> r1.pppg++;
		tl2.pppg++ <--> ethernet <--> r1.pppg++;
		tl3.pppg++ <--> ethernet <--> r1.pppg++;

		// control node connection
		cn.pppg++ <--> ethernet <--> r1.pppg++;
		 
 		globalFilter.nodes++ <--> tl1.global_filter;
 		globalFilter.nodes++ <--> tl2.global_filter;
 		globalFilter.nodes++ <--> tl3.global_filter;
 		globalFilter.nodes++ <--> r1.global_filter;
 		globalFilter.nodes++ <--> cn.global_filter;
        
}
