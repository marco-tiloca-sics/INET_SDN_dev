"""
Put class models the 'put' primitive. It puts a packet in the tx/rx buffer 
of a certain layer of a list of nodes.

The sintax of the primitive is: 
put(packetName, dstNodes, direction, updateStat, delay)
 + packetName is the name of the packet;
 + dstNodes is the list of the destination nodes of the packet;
 + direction is the direction of flow in the communication stack of the packet;
 + updateStat is the flag to enable/disable the update of stats;
 + delay is the forwarding delay.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Put:
    """
    put(packet, dstNodes, direction, updateStat, delay) 
    """
    
    argv = []
    argc = 5
    name = "Put"
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
                
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Put</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":nodes:" + self.argv[1] + ":direction:" + self.argv[2] + ":throughWC:" + self.argv[3] + ":delay:" + self.argv[4] + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
