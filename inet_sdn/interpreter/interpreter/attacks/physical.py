"""
The class PhysicalAttack models an XML block that supports the execution
of a physical attack.

Authors:
 + Alessandro Pischedda <alessandro.pischedda@gmail.com>
 + Marco Tiloca         <marco.tiloca84@gmail.com>
 + Francesco Racciatti  <racciatti.francesco@gmail.com>
"""


class PhysicalAttack:
    
    occurrence_time = ""
    involved_nodes = ""
    actions = []
    
    def __init__(self, occurrence_time, involved_nodes, actions):
        self.occurrence_time = occurrence_time
        self.involved_nodes = involved_nodes
        self.actions = list(actions)
    
    def __str__(self):
        xml = "\n\t\t<Attack>"
        xml += "\n\t\t\t<start_time>" + self.occurrence_time + "</start_time>"
        xml += "\n\t\t\t<node>" + self.involved_nodes + "</node>"
        
        for action in self.actions:
            xml += "\n" + str(action)
        
        xml += "\n\t\t</Attack>"
        xml += "\n"
        
        return xml
