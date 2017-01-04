package inet.applications.inet_sdn.smartGrid;


simple DataConcentratorApp {
    parameters:
        @display("i=block/app");
        
        int intervalReport @unit ("s") = default(2s);
        
        string localAddress = default(""); // may be left empty ("")
        int localPort = default(-1); // port number to listen on
        
        string connectAddress = default("");
        int connectPort = default(1000); 
        
        double startTime @unit(s) = default(0s); // time first session begins
        double stopTime @unit(s) = default(-1s);  // time of finishing sending, negative values mean forever
        
        @signal[sentPk](type=cPacket);
        @signal[rcvdPk](type=cPacket);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sentPk](title="packets sent"; source=sentPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="messageAge(rcvdPk)"; unit=s; record=histogram,vector; interpolationmode=none);
        
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}

