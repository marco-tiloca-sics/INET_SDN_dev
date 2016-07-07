"""
Clone class models the 'clone' primitive. It clones packets.

The sintax of the primitive is: 
clone(srcPacketName, dstPacketName)
 + srcPacketName is the name of the packet to clone;
 + dstPacketName is the name of the new (cloned) packet.
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Clone:
    """
    clone(srcPacketName, dstPacketName) 
    """
    
    name = "Clone"
    argv = []
    argc = 2
    
    def __init__(self, args):
        self.argv = args.split(':')
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Clone</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":newPacketName:" + self.argv[1] + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
