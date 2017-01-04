package inet.openflow.ctrlApps;

// Learning & Forwarding Switch behavior of OpenFlow controller.
// The Switch module learns the mapping of the source MAC address and the associated port.
// The Swtich takes forwarding decisions based on the type of the packet

simple FwdSwitch like CtrlBehavior
{
    parameters:

        @display("i=block/switch");
    gates:

}
