"""
The class ConditionalAttack models an XML block that supports the execution
of a conditional attack.

Authors:
 + Alessandro Pischedda <alessandro.pischedda@gmail.com>
 + Marco Tiloca         <marco.tiloca84@gmail.com>
 + Francesco Racciatti  <racciatti.francesco@gmail.com>
"""


class ConditionalAttack:

    occurrence_time = ""
    involved_nodes = ""
    actions = []
    variables_dictionary = {}
    packet_filter = ""
    
    def __init__(self, occurrence_time, involved_nodes, variables_dictionary, 
                 actions, packet_filter):
        self.occurrence_time = occurrence_time
        self.involved_nodes = involved_nodes
        self.actions = list(actions)
        self.variables_dictionary = dict(variables_dictionary)
        self.packet_filter = packet_filter
    
    def __str__(self):
        xml = "\n\t\t<Attack>"
        xml += "\n\t\t\t<start_time>" + self.occurrence_time + "</start_time>"
        xml += "\n\t\t\t<node>" + self.involved_nodes + "</node>"
        
        for key in self.variables_dictionary.keys():
            xml += "\n\t\t\t<var><name>" + str(key) + "</name>" + str(self.variables_dictionary[key]) + "</var>"
        
        xml += "\n\t\t\t<filter>" + self.packet_filter + "</filter>"
        
        for action in self.actions:
            xml += "\n" + str(action)
        
        xml += "\n\t\t</Attack>"
        xml += "\n"
        
        return xml
