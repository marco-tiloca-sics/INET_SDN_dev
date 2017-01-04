package inet.applications.inet_sdn.smartGrid;

simple DSOApp {
    parameters:
        @display("i=block/app");
        //acts like a server
        string localAddress = default("");
        int localPort = default(1025);
        
        @signal[rcvdPk](type=cPacket);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);

    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}

