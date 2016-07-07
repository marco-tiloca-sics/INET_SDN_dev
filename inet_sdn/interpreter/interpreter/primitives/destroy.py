"""
Destroy class models the 'destroy' primitive. It destroies a node.

The sintax of the primitive is: 
destroy(nodeId, time)
 + nodeId is the Id of the node to destroy;
 + time is the occurrence time.

NB: nodeId and the occurrence time are not stored in this object.

Authors:
 + Francesco Racciatti <racciatti.francesco@gmail.com>
 + Alessandro Pischedda	<alessandro.pischedda@gmail.com>
 + Marco Tiloca <marco.tiloca84@gmail.com>
"""


class Destroy:
    """
    destroy(nodeId, time)
    nodeId and time are not stored in this object
    """
    
    def __str__(self):
        xml = "\n\t\t\t<action>"
        xml += "\n\t\t\t\t<name>Destroy</name>"
        xml += "\n\t\t\t</action>"
        
        return xml
