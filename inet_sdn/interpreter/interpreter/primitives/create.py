"""
Create class models the 'create' primitive. It creates new packets.

The sintax of the primitive is: 
create(packetName, layer1.type, value1, layer2.type, value2, ...)
 + packetName is the name of the new packet;
 + layer1.type identifies the layer of the (external) packet;
 + value1 identifies the protocol of layer1.type;
 + layer2.type identifies the layer of the encapsulated packet;
 + value2 identifies the protocol of layer2.type;
 + ...

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Create:
    """
    create(packet, layer1.type, value1, layer2.type, value2, ...)
    """

    argv = []
    argc = 7 # at most
    name = "Create"
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if (len(self.argv) < 3):
            error_arguments(self.name, self.argc, len(self.argv))
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Create</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0]
        
        # Skip the first argument, i.e. the packet name
        i = 1
        while (i < len(self.argv)):
            # Remove '"' from string arguments
            field = self.argv[i].replace('"',"") 
            xml += ":" + field
            i = i + 1
        
        xml += "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
