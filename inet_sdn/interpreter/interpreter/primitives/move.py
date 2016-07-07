"""
Move class models the 'move' primitive. It moves a node.

The sintax of the primitive is: 
move(nodeId, time, x, y, z)
 + nodeId is the Id of the node to move;
 + time is the occurrence time;
 + <x,y,z> are the destination coords of the node.

NB: nodeId and the occurrence time are not stored in this object.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


from utilities import error_arguments


class Move:
    """
    move(nodeId, time, x, y, z)
     + nodeId and time are not stored in this object
    """

    argv = []
    argc = 3	
    name = "Move"
    
    def __init__(self, args):
        self.argv = args.split(":")
    
    def __str__(self):
        if self.argc != len(self.argv):
            error_arguments(self.name, self.argc, len(self.argv))
        
        coordinates = self.argv[0] + ":" + self.argv[1] + ":" + self.argv[2]
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Move</name>"
        xml += "\n\t\t\t\t<parameters>" + coordinates + "</parameters>"
        xml += "\n\t\t\t</action>"
        
        return xml
