//	
// author: Francesco Racciatti
// email: racciatti.francesco@gmail.com
//


package inet.localfilter;


// The LocalFilter is a module which interposes between other modules to catch all passing messages

simple LocalFilter{

	gates: 
		// TODO
		// local filter gate name convention
		// local filter is interposed between two sub-modules, generally belonging to two different levels
		// i.e. one sub-module belongs to a higher level than the other
		// [A] <--> [localfilter] <--> [B]
		//          ^            ^
		//          A_B_sup      A_B_inf			 
		
		
	//local filter between layer 5 and layer 4
		// local filter between app(udp) and udp
		inout app_udp_sup[];
		inout app_udp_inf[];
		
		// local filter between app(tcp) and tcp
		inout app_tcp_sup[];
		inout app_tcp_inf[];

		// local filter between app(sctp) and sctp
		inout app_sctp_sup[];
		inout app_sctp_inf[];
		
		
	//local filter between layer 4 and layer 3
		// local filter between udp and net
		inout udp_net_sup[];
		inout udp_net_inf[];

		// local filter between tcp and net
		inout tcp_net_sup[];
		inout tcp_net_inf[];
		

	//local filter between layer 3 and layer 2
		// local filter between net and ppp
		inout net_ppp_sup[];
		inout net_ppp_inf[];

		// local filter between net and eth
		inout net_eth_sup[];
		inout net_eth_inf[];


	//local filter between layer 2 and layer 1
		// local filter between ppp and physical
		inout ppp_phy_sup[];
		inout ppp_phy_inf[];

		
		// gate from/to global filter
		inout global_filter;
		
		// gate to logger
		output to_logger;	
	
}







