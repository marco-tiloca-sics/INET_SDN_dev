package inet.applications.inet_sdn.smartGrid;

simple RTUApp {
    parameters:
        @display("i=block/app");
        //acts like a server
        string localAddress = default("");
        int localPort = default(1000);
        int intervalReport = default(3);
        
        string connectAddress = default("");
        int connectPort = default(1025); 
        
        double startTime @unit(s) = default(0s); // time first session begins
        
        @signal[sentPk](type=cPacket);
        @signal[rcvdPk](type=cPacket);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sentPk](title="packets sent"; source=sentPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="messageAge(rcvdPk)"; unit=s; record=histogram,vector; interpolationmode=none);
        
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}

