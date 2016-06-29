"""
Change class models the 'change' primitive. It changes the content of a field
of a packets.

The sintax of the primitive is: 
change(packetName, field, value)
 + packetName is the name of the target packet;
 + field has the format 'layerName.fieldName':
    + layerName is the name of the layer in which change the field;
    + fieldName is the name of the field to change.
 + value is the new value of the field.
 
Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Change:
    """
    change(packetName, field, value)
    """
    
    argv = []
    argc = 3
    name = "Change"
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        field = self.argv[1].replace('"', "")
        
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Change</name>"
        xml += "\n\t\t\t\t<parameters>packetName:" + self.argv[0] + ":field_name:" + field + ":value:" + self.argv[2] + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
