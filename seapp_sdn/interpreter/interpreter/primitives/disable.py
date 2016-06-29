"""
Disable class models the 'disable' primitive. It disables a node.

The sintax of the primitive is: 
disable(nodeId, time)
 + nodeId is the Id of the node to disable;
 + time is the occurrence time.

NB: nodeId and the occurrence time are not stored in this object.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


class Disable:
    """
    disable(nodeId, time)
    nodeId and time are not stored in this object
    """
    
    def __str__(self):
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Disable</name>"
        xml += "\n\t\t\t</action>"
        
        return xml
