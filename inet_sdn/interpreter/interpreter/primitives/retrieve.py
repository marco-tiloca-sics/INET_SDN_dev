"""
Retrieve class models the 'retrieve' primitive. It retrieves the content
of a field of a packet.

The sintax of the primitive is: 
retrieve(packetName, field, variable)
 + packetName is the name of the target packet;
 + field has the format 'layerName.fieldName':
    + layerName is the name of the layer in which search the field;
    + fieldName is the name of the field to seach.
 + variable is the name of the variable in which store the retrieved value.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Retrieve:
    """
    retrieve(packetName, field, variable)
    """
    
    argv = []
    argc = 3
    name = "Retrieve"
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        field = self.argv[1].replace('"', "")
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Retrieve</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":field_name:" + field + ":varName:" + self.argv[2] + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
