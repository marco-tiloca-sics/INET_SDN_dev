"""
Drop class models the 'drop' primitive. It drops packets.

The sintax of the primitive is: 
drop(packetName, threshold)
 + packetName is the packet to drop;
 + threshold defines the occurrence probability of the drop action.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Drop:
    """
    drop(packetName, threshold)
    """
    
    name = "Drop"
    argv = []
    argc = 2
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        if float(self.argv[1]) > 1.0:
            sys.exit("Error: drop() threshold argument must belong to the range [0,1] ")
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Drop</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":threshold:" + self.argv[1] + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
