"""
The class UnconditionalAttack models an XML block that supports the execution
of an unconditional attack.

Authors:
 + Alessandro Pischedda <alessandro.pischedda@gmail.com>
 + Marco Tiloca         <marco.tiloca84@gmail.com>
 + Francesco Racciatti  <racciatti.francesco@gmail.com>
"""


class UnconditionalAttack:
    
    occurrence_time = ""
    actions = []
    variables_dictionary = {}
    frequency = ""
    
    def __init__(self, occurrence_time, frequency, variables_dictionary,
                 actions):
        self.occurrence_time = occurrence_time
        self.actions = list(actions)
        self.variables_dictionary = dict(variables_dictionary)
        self.frequency = frequency
    
    def __str__(self):
        xml = "\n\t\t<Attack>"
        xml += "\n\t\t\t<start_time>" + self.occurrence_time + "</start_time>"
        xml += "\n\t\t\t<frequency>" + self.frequency + "</frequency>"
        
        for key in self.variables_dictionary.keys():
            xml += "\n\t\t\t<var><name>" + str(key) + "</name>" + str(self.variables_dictionary[key]) + "</var>"
        
        for action in self.actions:
            xml += "\n" + str(action)
        
        xml += "\n\t\t</Attack>"
        xml += "\n"
        
        return xml
